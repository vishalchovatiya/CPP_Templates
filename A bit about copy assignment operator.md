### Brief
- Here we are going to learn about copy assignment operator. Although I am not an expert but this what I have learned so far from various sources. So this article is basically a collection of connected dots while i was introducing C++ to myself.
- We will not learn basic things. I am not going to tell you that this is basic function compiler provides if you dont define that in your class nor i will tell calling & synthesis of this function. We will learn why it is needed & basic function to have in your class, why it like that only & what it should look like. So let's start.
### Why we need it? <----- all time my faviroute quesion.

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
- While i was learning about copy assignment operator, i always have doubt why we return value from copy assignment operator function. We dont need to, if you look at above code we are already assigning to current object i.e. `x2` who called copy assignment operator function. I can understand the need of `const` in argument of copy assignment operator function. But return value was not justifiable to me till i saw following code:
```
X x1(1), x2(2), x3(3);
x3 = x2 = x1;
```
- If you make return type of copy assignment operator function as `void`, compiler wont throw error till you are using `x2 = x1;`.But when assignment chain will be created you have to return something so that it can be argument on further call to copy assignment operator.
- So we find out that we have return something from copy assignment operator to support assignment chainning feature. But what should be appropriate return type? This will lead us to our next topic as follows.

### What should be the appropriate return type of copy assignment operator?
https://www.youtube.com/watch?v=9WPUU9Uzaus
### References
- https://stackoverflow.com/questions/3105798/why-must-the-copy-assignment-operator-return-a-reference-const-reference
