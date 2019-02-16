### Brief
- Here we are going to learn about copy assignment operator. Although I am not an expert but this what I have learned so far from various sources. So this article is basically a collection of connected dots while i was introducing C++ to myself.
- We will not learn basic things. I am not going to tell you that this is basic function compiler provides if you dont define that in your class nor i will tell calling & synthesis of this function. We will learn why it is needed & basic function to have in your class, why it like that only & what it should look like. So let's start.
### Why we need it? <----- my all time favriote quesion.

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

  x2 = x1;          // Statement 1: Correct & works fine 
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
main.cpp:13:5: note: candidate function not viable: expects an l-value for 1st argument
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

### addition & substraction operator overload

### References
- https://stackoverflow.com/questions/3105798/why-must-the-copy-assignment-operator-return-a-reference-const-reference
