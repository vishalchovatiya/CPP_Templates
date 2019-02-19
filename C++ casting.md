### Brief
- Here we are going to learn about the type-casting. Although I am not an expert but this what I have learned so far from various sources. So this article is basically a collection of connected dots while I was introducing C++ to myself.
- In C++, there are 5 different types of casts: C-style casts, static casts, const casts, dynamic casts, and reinterpret casts.
- As usual, we will start with `why we need it?`
### why we need it?
There are two basic types of type conversion: 
  1. Implicit conversion : where the compiler automatically transforms one fundamental data type into another, hence also called automatic type conversion.
  2. Explicit conversions, where the developer uses a casting operator to direct the conversion.
All types of casting falls under explicit type conversions.



Const casts and reinterpret casts should generally be avoided because they are only useful in rare cases and can be harmful if used incorrectly.

Rule: Avoid const casts and reinterpret casts unless you have a very good reason to use them.

```
#include <iostream>
using namespace std;

class A {};

class B {
public:
  B (const A& x) {
    cout<<"CC\n";
  }
  B& operator= (const A& x) {
    cout<<"ASSIGNMENT\n";
    return *this;
  }
  operator A() {
    cout<<"TYPE-CAST\n";
    return A();
  }
};

int main ()
{
  A foo;
  B bar = foo;    // calls constructor
  bar = foo;      // calls assignment
  foo = bar;      // calls type-cast operator
  return 0;
}

```

### C-style casts
```
int main() { 
    int var_1 = 10;
    int var_2 = 4;
    float res = var_1 / var_2;
    cout<<res<<"\n";
    return 0; 
}
```
- When you will try to run above code, you will get `2` as output which we dont want. To initialize `res` variable correctly we need to type cast using float as follows:
```
float res = (float)var_1 / var_2;
```
- Now your answer will be `2.5`. This type of casting is very simple & staight forward as it appear.
- You can also write above casting in C++ as:
```
float res = float(var_1) / var_2;
```
- C-style casts can change a data type without changing the underlying representation which may lead to garbage results. You should only use C-style cast when you know the data type & data representation of `lvalue` & `rvalue` good enough.

### static_cast
- The main advantage of static_cast is that it provides compile-time type checking, making it harder to make an inadvertent error. 
```
int i = 48;
char ch = i; // implicit conversion
```
Casting an int (4 bytes) to a char (1 byte) is potentially unsafe, and the compiler will typically complain. In order to announce to the compiler that you are explicitly doing something you recognize is potentially unsafe (but want to do anyway), you can use a cast:
```
int i = 48;
char ch = static_cast<char>(i);
```
- Let's understand this with C++ example:
```
class Derive : public Base {...};
class OtherClass {...} ;

Base  *pSomething; // filled somewhere, Can point to Base or Derived object
```
- Now, these two are compiled the same way:
```
Derive *pDerive;

pDerive = static_cast<Derive*>(pSomething);       // Safe, checked at run time by static_cast
pDerive = (Derive*)(pSomething);                  // coercion/forceful conversion
```
- However, let's see this almost identical code:
```

OtherClass *pOtherClass;

pOtherClass = static_cast<OtherClass*>(pSomething); // Compiler error: Can't convert
pOtherClass = (OtherClass*)(pSomething);            // No compiler error. and it's wrong!!!
```

### References
- https://www.learncpp.com/cpp-tutorial/4-4a-explicit-type-conversion-casting/
- https://www.learncpp.com/cpp-tutorial/44-implicit-type-conversion-coercion/
- https://stackoverflow.com/questions/103512/why-use-static-castintx-instead-of-intx
