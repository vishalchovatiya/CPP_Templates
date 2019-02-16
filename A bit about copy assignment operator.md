### Brief
- Here we are going to learn about copy assignment operator. Although I am not an expert but this what I have learned so far from various sources. So this article is basically a collection of connected dots while i was introducing C++ to myself.
- We will not learn basic things. I am not going to tell you that this is basic function compiler provides if you dont define that in your class nor i will tell calling & synthesis of this function. We will learn why it is needed & basic function to have in your class, why it like that only & what it should look like. So let's start.
### Why we need it? <----- my all time favriote quesion.
- Simple answer is just to assign data. As we do assignment in primitive data like 'int a; a = 5' some times we also need to do this in our user defined data type i.e. class.
- Class could be complex entity so that we need special function which does this task. Although compiler provide default copy constuctor. But in some cases you have to define your own copy assignment operator function like:
  1. Write your own assignment operator that does deep copy if you are using dynamic memory.
  2. Do not allow assignment of one object to other object. We can create our own dummy assignment operator and make it private.
### Why we need to return something from copy assignment operator?
```
class X{
public:
  int var;
  X(int x){this->var = x;}
  X& operator=(const X &rhs){
    this->var = rhs.var;
    return *this;
  }
};

X x1(1), x2(2);
x2 = x1;          // Probably be transform into X::x2.operator=(x1);
```
- While i was learning about copy assignment operator, i always have doubt why we return value from copy assignment operator function. We dont need to, if you look at above code we are already assigning to current object i.e. `x2` using `this` pointer who called copy assignment operator function. I can understand the need of `const` in argument of copy assignment operator function. But return value was not justifiable to me till i saw following code:
```
X x1(1), x2(2), x3(3);
x3 = x2 = x1;
```
- If you make return type of copy assignment operator function as `void`, compiler wont throw error till you are using `x2 = x1;`.But when assignment chain will be created like `x3 = x2 = x1;` you have to return something so that it can be argument on further call to copy assignment operator.
- So it is to return something from copy assignment operator to support assignment chainning feature. But what should be appropriate return type? This will lead us to our next topic as follows.

### What should be the appropriate return type of copy assignment operator?
- I know, you will say we have to return reference to current object in which we have assigned rvalue & yeh! that's corrent also but why not return by value or pointer ?

> **Let's try `return by value`**

```
class X{
public:
  int var;

  X(int x){this->var = x;}

  X operator = (X &rhs){
    this->var = rhs.var;
    return *this;
  }  
};

int main(){
  X x1(1), x2(2), x3(3);

  x2 = x1;          // Statement 1: Works fine 
  (x3 = x2) = x1;   // Statement 2: Correct, but meaning less statement
  x3 = (x2 = x1);   // Statement 3: Meaningful but compiler won't alllow us
  x3 = x2 = x1;     // Statement 4: Meaningful but compiler won't alllow us

  cout<<x1.var<<"\n";
  cout<<x2.var<<"\n";
  cout<<x3.var<<"\n";
  
  return 0;
 }
```
- When you will compile above code, gcc will throw error as follows:
```
error: no viable overloaded '='
  x3 = (x2 = x1);   // Statement 3: Meaningful but compiler won't alllow us
  ~~ ^ ~~~~~~~~~
note: candidate function not viable: expects an l-value for 1st argument
  X operator = (X &rhs){
    ^
```
- Lets understand all these statements one-by-one
```
x2 = x1;          // Statement 1: Correct & works fine
```
- This is correct & works fine as we are not using return value provided by copy assignment operator fiunction anywhere else.
```
(x3 = x2) = x1;   // Statement 2: Correct, but meaning less statement
```
- This statements is perfectly fine & have no prblem in compilation. But `Statemetn 2` is meaning less as we are first assigning `x2` into `x3` which returns temporary object which again calls copy assignment operator with `x1` as argument. This works fine but at the end call of copy assignment operator, we are assigning value of `x1` to temporary object which is meaning less thing.
- Probable transformation of `Statemetn 2` by compiler would be
```
(X::x3.operator=(x2)).operator=(x1);
```
- With more simplicity
```
X temp = X::x3.operator=(x2);
X::temp.operator=(x1);
```
- As you can see i have taken `temp` object which usually created by compiler as we are returning object by value. So this way output would be `1 2 2` which is not correct. Now we will observe `Statement 3`
```
x3 = (x2 = x1);   // Statement 3: Meaningful but compiler won't alllow us
```
- Probable transformation of `Statemetn 3` by compiler would be
```
(X::x3.operator=((x2 = x1));
```
- Code till operation `x2 = x1` is fine but when the result of that operation becomes argument to another copy assignment operator function, it will create problem of temporary object binding to non-const reference. If you dont know about "temporary object binding to non-const reference" then you should find out the reason behind why following program is not working, you will understand everything you wanted to know for `Statement 3`.
```
int main() {
  const string& val1 = string("123");   // Works fine
  string& val2 = string("123");         // Will throw error
  return 0;
}
```
- Error:
```
clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
exit status 1
error: non-const lvalue reference to type 'basic_string<...>' cannot bind to a temporary of type 'basic_string<...>'
  string& val2 = string("123");
          ^      ~~~~~~~~~~~~~
1 error generated.
```
- Note that above code will work in some of old compiler like VC2012,etc. Now we will move to `Statement 4`
```
x3 = x2 = x1;     // Statement 4: Meaningful but compiler won't alllow us
```
- This will also throw same error as `Statement 3`, because conceptually both are same. Although `Statement 3` & `Statement 4` can also be valid if you modify argument of copy assignement ooperator from `pass by reference` to `pass by value` which we know unnecessary overhead of calling copy constructor which also stands true for return type.

> **Let's try `return by pointer`**
```
class X{
public:
  int var;

  X(int x){this->var = x;}

  X* operator = (X &rhs){
    this->var = rhs.var;
    return this;
  }  
};

int main(){
  X x1(1), x2(2), x3(3);

  x2 = x1;          // Statement 1: Works fine 
  x3 = x2 = x1;     // Statement 4: Meaningful but compiler won't alllow us

  cout<<x1.var<<"\n";
  cout<<x2.var<<"\n";
  cout<<x3.var<<"\n";
  
  return 0;
 }
```
- This time we will not observe for all four statements rather will go for 2 basic statement which are also valid for primitive data.
- `Statement 1` is not correct but still works fine. While `Statement 4` throws error
```
clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
exit status 1
error: no viable overloaded '='
  x3 = x2 = x1;     // Statement 4: Meaningful but compiler wont alllow us
  ~~ ^ ~~~~~~~
note: candidate function not viable: no known conversion from 'X *' to 'X &' for 1st argument; dereference the argument with *
  X* operator = (X &rhs){
     ^
1 error generated.
```
- Probable transformation of `Statemetn 4` by compiler would be
```
(X::x3.operator=( ( x2 = x1 ) );
```
- This will not work simply because result of operation `( x2 = x1 )` is pointer & copy assignement operator function wants reference as argument.
- Now you will tell me that why we just not change arguemtn with pointer rather than accepting it as reference. Nice idea! i would say
```
X* operator = (X *rhs){
  cout<<"THIS\n";
  this->var = rhs->var;
  return this;
}  
```
- Now to call above copy assignment operator you need to use following operation
```
x2 = &x1;
```
- Because we are expecting pointer as argument in copy assignment operator. `x1 = x2` or `x3 = x2 = x1` wont work anymore. If you are still gettin correct answer as `1 1 1` in your output window then just consider print from `cout`. You are getting correct answer `1 1 1` because default copy constructor is getting called everytime, we have just overloaded copy constructor by changing its return type & argument as pointer.

> **Conclusion**
- Above are the reason why it is not feasible to use `pass by value` or `pointer` in argument or return type of copy assignment operator.
- Compiler designer have designed standard in such a way that your class object should also work same as primitive types like
```
// Primitive type & operations
int a = 5, b, c;
a = b = c;
// User defined types & operations
X x1(5), x2, x3;
x3 = x2 = x1;
```

### addition & substraction operator overload

### References
- https://stackoverflow.com/questions/3105798/why-must-the-copy-assignment-operator-return-a-reference-const-reference.
- https://www.geeksforgeeks.org/assignment-operator-overloading-in-c/
