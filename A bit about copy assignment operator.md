### Brief
- Here we are going to learn about the copy assignment operator. Although I am not an expert but this what I have learned so far from various sources. So this article is basically a collection of connected dots while I was introducing C++ to myself.
- We will not learn basic things. I am not going to tell you that this is basic function compiler provides if you don't define that in your class nor I will tell calling & synthesis of this function. We will learn why it is needed & basic function to have in your class, why it like that only & what it should look like. So let's start.
### Why we need it? <----- my all time favourite question.
- The simple answer is just to assign data. As we do an assignment in primitive data like `int a; a = 5` sometimes we also need to do this in our user-defined data type i.e. class. Answer would be same as copy constructor which i have given here[TODO].
- A class could be a complex entity so that we need a special function which does this task. Although compiler provides a default one. But in some cases, you have to define your own copy assignment operation function like:
  1. Write your own assignment operator that does the deep copy if you are using dynamic memory.
  2. Do not allow the assignment of one object to another object. We can create our own dummy assignment operator and make it `private` or simply `delete` it.
### Why we need to return something from the copy assignment operator?
- While I was learning about the copy assignment operator, I always have doubt why we need to return value from the copy assignment operator function.
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
- We don't need to, if you look at the above code we are already assigning to current object i.e. `x2` using `this` pointer who called copy assignment operator function. I can understand the need for `const` in the argument of the copy assignment operator function. But return value was not justifiable to me until i saw the following code:
```
X x1(1), x2(2), x3(3);
x3 = x2 = x1;
```
- If you make return type of copy assignment operator function as `void`, the compiler won't throw error till you are using `x2 = x1;`.But when assignment chain will be created like `x3 = x2 = x1;` you have to return something so that it can be an argument on further call to copy assignment operator.
- So we have to return something from the copy assignment operator to support assignment chaining feature. But what should be the appropriate return type? This will lead us to our next point.

### What should be the appropriate return type of copy assignment operator?
- I know, you will say we have to return reference of the current object. Yeh! that's correct also but why not `return by value` or `pointer`?

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
- When you will compile the above code, GCC will throw an error as follows:
```
error: no viable overloaded '='
  x3 = (x2 = x1);   // Statement 3: Meaningful but compiler won't alllow us
  ~~ ^ ~~~~~~~~~
note: candidate function not viable: expects an l-value for 1st argument
  X operator = (X &rhs){
    ^
```
- Let's understand all these statements one-by-one
```
x2 = x1;          // Statement 1: Works fine
```
- This is not correct but still works fine as we are not using return value provided by copy assignment operator function anywhere else.
```
(x3 = x2) = x1;   // Statement 2: Correct, but meaningless statement
```
- This statements is perfectly fine & have no problem in a compilation. But `Statement 2` is meaningless as we are first assigning `x2` into `x3` which returns a temporary object which again calls copy assignment operator with `x1` as an argument. This works fine but at the end call of the copy assignment operator, we are assigning the value of `x1` to a temporary object which is meaningless.
- Probable transformation of `Statement 2` by compiler would be
```
(X::x3.operator=(x2)).operator=(x1);
```
- With more simplicity
```
X temp = X::x3.operator=(x2);
X::temp.operator=(x1);
```
- As you can see I have taken `temp` object which usually created by the compiler as we are returning an object by value. So this way output would be `1 2 2` which is not correct. Now we will observe `Statement 3`
```
x3 = (x2 = x1);   // Statement 3: Meaningful but compiler won't allow us
```
- Probable transformation of `Statement 3` by compiler would be
```
(X::x3.operator=((x2 = x1));
```
- Code till operation `x2 = x1` is fine but when the result of that operation becomes an argument to another copy assignment operator function, it will create the problem of temporary object binding to a non-const reference. 
- If you don't know about "temporary object binding to non-const reference" then you should find out the reason behind why the following program is not working, you will understand everything you wanted to know for `Statement 3`.
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
- Note that the above code will work in some of the old compilers like VC2012, etc. Now we will move to `Statement 4`
```
x3 = x2 = x1;     // Statement 4: Meaningful but compiler won't allow us
```
- This will also throw the same error as `Statement 3` because conceptually both are same. Although `Statement 3` & `Statement 4` can also be valid if you modify argument of copy assignment operator from `pass by reference` to `pass by value` which we know adds the unnecessary overhead of calling copy constructor which also stands true for the return type.

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
- This time we will not observe all four statements rather will go for 2 basic statement which is also valid for primitive data types.
- `Statement 1` is not correct but still works fine. While `Statement 4` throws an error
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
- Probable transformation of `Statement 4` by compiler would be
```
(X::x3.operator=( ( x2 = x1 ) );
```
- This will not work simply because the result of operation `( x2 = x1 )` is pointer & copy assignment operator function wants a reference as an argument.
- Now you will say that why we just not change argument with pointer rather than accepting it as a reference. Nice idea! I would say
```
X* operator = (X *rhs){
  cout<<"THIS\n";
  this->var = rhs->var;
  return this;
}  
```
- Now to call above copy assignment operator you need to use the following operation
```
x2 = &x1;
```
- Because we are expecting pointer as an argument in copy assignment operator. `x1 = x2` or `x3 = x2 = x1` wont work anymore. 
- If you are still getting the correct answer as `1 1 1` in your output window then just consider print from `cout`. You are getting the correct answer `1 1 1` because default copy constructor provided by the compiler is getting called every time. Technically, we have just overloaded copy constructor by changing its return type & argument as a pointer.

> **Conclusion**
- Above are the reason why it is not feasible to use `pass by value` or `pointer` an argument or return type of copy assignment operator.
- Compiler designer have designed standard in such a way that your class object operation should also work same as primitive type operations like
```
// Primitive type & operations
int a = 5, b, c;
a = b = c;

// User defined type & operations
X x1(5), x2, x3;
x3 = x2 = x1;
```
### References
- https://stackoverflow.com/questions/3105798/why-must-the-copy-assignment-operator-return-a-reference-const-reference.
- https://www.geeksforgeeks.org/assignment-operator-overloading-in-c/
