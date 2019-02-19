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
  operator A() {
    cout<<"TYPE-CAST\n";
    return A();
  }
};

int main ()
{
  A foo;
  B bar;
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
- The main advantage of static_cast is that it provides compile-time type checking, making it harder to make an inadvertent error. Let's understand this with C++ example:
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
- As you can see, there is no easy way to distinguish between the two situations without knowing a lot about all the classes involved.
- The second problem is that the C-style casts are too hard to locate. In complex expressions it can be very hard to see C-style casts. 

### References
- https://www.learncpp.com/cpp-tutorial/4-4a-explicit-type-conversion-casting/
- https://www.learncpp.com/cpp-tutorial/44-implicit-type-conversion-coercion/
- https://stackoverflow.com/questions/103512/why-use-static-castintx-instead-of-intx
