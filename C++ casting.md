### Brief
- Here we are going to learn about the type-casting. Although I am not an expert but this what I have learned so far from various sources. So this article is basically a collection of connected dots while I was introducing C++ to myself.
- In C++, there are 5 different types of casts: C-style casts, static casts, const casts, dynamic casts, and reinterpret casts.
- I have tried to cover most of the intricacies to clear main concept, but still there might be chance that i may miss some.


- As usual, we will start with `why we need it?`

### Jargons you need to face
  1. Implicit conversion : where the compiler automatically transforms one fundamental data type into another, hence also called automatic type conversion. Like `float f = 3;`, here compiler will not complain but directly transform 3 which is of type integer into float & assign to `f`.
  2. Explicit conversions, where the developer uses a casting operator to direct the conversion. All types of manual casting falls under explicit type conversions catogory. Like `int * p = (int*)malloc(10);`, here we explicitly casting `void*` to `int*`.
  3. l-value = locator value = identifier which represents memory location. For example, variable name, `*ptr` where ptr points to memory location, etc.
  4. r-value = value which is not l-value = value appear on right hand side of assignment(=) operator. Like `int a = 5; // 5 = r-value`, `q = p + 5; // p + 5 is r-value`, etc.
  
  Note: Although there are some exceptions in l-value & r-value which i have not discussed here. And it is also not easy to understand as it seems but i have tried to make it easy for you to remember.

### Why we need it?




Const casts and reinterpret casts should generally be avoided because they are only useful in rare cases and can be harmful if used incorrectly.

Rule: Avoid const casts and reinterpret casts unless you have a very good reason to use them.


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
- If you from C background like me, then this will be your best goto cast. For example:
```
int * p = malloc(10);
```
- When you try to compile above code using C compiler it works fine. But C++ compiler is not kind enough. It will throw error as follows :
```
exit status 1
error: cannot initialize a variable of type 'int *' with an rvalue of type 'void *'
  int * p = malloc(10);
        ^   ~~~~~~~~~~
1 error generated.
```
- The first thing that comes to your mind is our friend, the C-style cast:
```
int * p = (int*)malloc(10);
```
- This will work, but this style of cast is not recommended in C++. static_cast handles implicit conversions like this. We will primarily use it for converting in places where implicit conversions fail, such as malloc.
```
int * y = static_cast<int*>(malloc(10));
```
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
- The second problem is that the C-style casts are too hard to locate/visualize. In complex expressions it can be very hard to see C-style casts e.g. `T(something)` syntax is equivalent to `(T)something`.
- Following example is also one of the use case & self-explanatory:
```
int main ()
{
  char c = 10;       // 1 byte
  int *p = (int*)&c; // 4 bytes

  *p = 5; // stack corruption

  int *q = static_cast<int*>(&c); // compile-time error

  return 0;
}
```
### const_cast
- Rather going about description on const_cast, why dont we start seeing usecases.
> **1. Not allowed to const_cast variables that are actually const**
 - You are not allowed to const_cast variables that are actually const. This results in undefined behavior. const_cast is used to remove the const-ness from references and pointers that ultimately refer to something that is not const.
- So, this is allowed:
```
int i = 0;
const int& ref = i;
const int* ptr = &i;

const_cast<int&>(ref) = 3;
*const_cast<int*>(ptr) = 3;
```
- It's allowed because i, the object being assigned to, is not const. The below is not allowed:
```
const int i = 0;
const int& ref = i;
const int* ptr = &i;

const_cast<int&>(ref) = 3;
*const_cast<int*>(ptr) = 3;
```
- Because here i is const and you are modifying it by assigning it a new value. The code will compile, but its behavior is undefined (which can mean anything from "it works just fine" to "the program will crash".)
> **2. Modifying data member using `const` `this` pointer**
- const_cast can be used to change non-const class members by method in which use declared this pointer as const. See following example:
```
class X {
public:
  int var;

  void changeAndPrint(int temp) const
  {
    this->var = temp;   // Throw compilation error

    (const_cast<X*>(this))->var = temp; // Works fine    
  }
};

int main ()
{
  X x;
  x.changeAndPrint(5);
  cout<<var<<"\n";
  return 0;
}
```
- This can also be useful when overloading member functions based on const, for instance:
```
class X {
public:
  int var;

  void changeAndPrint(int temp) const
  {
    (const_cast<X*>(this))->var = temp; // Works fine    
  }

  void changeAndPrint(int *temp)
  {
    // Do some stuff
  }
};

int main ()
{
  int a = 4;
  X x;

  x.changeAndPrint(&a);
  x.changeAndPrint(5);

  cout<<x.var<<"\n";
  
  return 0;
}
```
> **3. Pass const argument to function which accept only non-const argument**
- const_cast can also be used to pass const data to a function that doesn’t receive const argument. See following code:
```
int fun(int* ptr) 
{ 
    return (*ptr + 10); 
} 
  
int main(void) 
{ 
    const int val = 10; 
    cout << fun(const_cast <int *>(&val)); 
    return 0; 
} 
```
> **4. Cast away volatile attribute**
- const_cast can also be used to cast away volatile attribute. For example:
```
int main(void) 
{ 
    int a1 = 40; 
    const volatile int* b1 = &a1; 
    cout << "typeid of b1 " << typeid(b1).name() << '\n'; 
    int* c1 = const_cast <int *> (b1); 
    cout << "typeid of c1 " << typeid(c1).name() << '\n'; 
    return 0; 
} 
```
- output
```
typeid of b1 PVKi
typeid of c1 Pi
```
- Here, 
PVKi (pointer to a volatile and constant integer) 
Pi (Pointer to integer)
Whatever we have discussed above in const_cast is also valid for volatile keyword.

### dynamic cast
- dynamic_cast uses the type checking at runtime in oppose to static_cast which does it compile time. dynamic_cast is more usefull when you dont know the type of input is representing to. Let assume:
```
Base* CreateRandom()
{
    if( (rand()%2) == 0 )
        return new Derived1;
    else
        return new Derived2;
}

Base* base = CreateRandom();
```
- You dont know which object will be return by `CreateRandom()` at run time but you want to execute `Method1()` of `Derived1` if it returns `Derived1`. So in this scenario you can use dynamic_cast as follows
```
Derived1 *pD1 = dynamic_cast<Derived1*>(base);
if(pD1){
  pD1->Method1();
}
```
- In case if `base` does not point to `Derived1`, it will return nullptr for pointers or throw a std::bad_cast exception for references. In order to work with dynamic_cast, your classes must be polymorphic type i.e. must include 'virtual' at least one methods.
- dynamic_cast take advantage of RTTI(Run Time Type Identification) mechanism which we have already learned a bit in our previous articles.

### reinterpret_cast
- reinterpret_cast is a compiler directive which tells the compiler to treat the current type as a new type.
- You can use reinterpret_cast to cast any pointer or integral type to any other pointer or integral type. 
- This can lead to dangerous situations: nothing will stop you from converting an int to a std::string *.
- You will use reinterpret_cast in your embedded systems. A common scenario where reinterpret_cast applies is converting between uintptr_t and an actual pointer or between:
```
error: static_cast from 'int *' to 'uintptr_t'
      (aka 'unsigned long') is not allowed
        uintptr_t ptr = static_cast<uintptr_t>(p);
                        ^~~~~~~~~~~~~~~~~~~~~~~~~
1 error generated.
```
- Instead, use this:
```
uintptr_t ptr = reinterpret_cast<uintptr_t>(p);
```

### Cheat code for C developers moving to C++ on casting
After reading all this you may confuse on what to use & when! That's why i have created this cheatcode 
- Avoid C-style casts. Be sure on what you want while casting.
- Use static_cast wherever you were using C-style cast.
- Use reinterpret_cast for specific cases were you need to reinterpret underlying data (e.g. converting from a pointer to uintptr_t)
- Use dynamic_cast wherever you casting pointers and references which points to inheritance hierarchy. Keep in mind that only use dynamic_cast on classes with atleast one virtual member.
- Use const_cast when you need to remove const or volatile keywords. Think carefully before using this cast.


### References
- https://www.learncpp.com/cpp-tutorial/4-4a-explicit-type-conversion-casting/
- https://www.learncpp.com/cpp-tutorial/44-implicit-type-conversion-coercion/
- https://stackoverflow.com/questions/103512/why-use-static-castintx-instead-of-intx
- https://www.geeksforgeeks.org/const_cast-in-c-type-casting-operators/
- https://stackoverflow.com/questions/2253168/dynamic-cast-and-static-cast-in-c
