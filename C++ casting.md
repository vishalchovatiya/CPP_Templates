### Brief
- Here we are going to learn about the type-casting. Although I am not an expert but this what I have learned so far from various sources. So this article is basically a collection of connected dots while I was introducing C++ to myself.
- In C++, there are 5 different types of casts: C-style casts, `static_cast`, `const_cast`, `dynamic_cast`, and `reinterpret_cast`.
- I have tried to cover most of the intricacies to clear the main concept, but still, there might be a chance that I may miss some.
- I usually start with `why we need it?`, but this time first we quickly go through some jargons

### Jargons you need to face
  1. Implicit conversion = where the compiler automatically transforms one fundamental data type into another, hence also called automatic type conversion. Like `float f = 3;`, here compiler will not complain but directly transform `3` which is of type integer into float & assign to `f`.
  2. Explicit conversions = where the developer uses a casting operator to direct the conversion. All types of manual casting falls under explicit type conversions catogory. Like `int * p = (int*)malloc(10);`, here we explicitly casting `void*` to `int*`.
  3. `l-value` = locator value = identifier which represents memory location. For example, variable name, `*ptr` where ptr points to memory location, etc.
  4. `r-value` = value which is not `l-value` = value appear on right hand side of assignment(`=`) operator. Like `int a = 5; // 5 = r-value`, `q = p + 5; // p + 5 is r-value`, etc.
  
Note: Although there are some exceptions & more to learn on `l-value` & `r-value` which I have discussed here[TODO]. 

### Why we need it?

- Data is a representation of the bits(`0`s & `1`s) in memory.
- Data-type is compiler directive which tells the compiler how to store particular data.
- `unsigned int a = 5;` by this statement you can presume that 4 bytes will be reserved in your memory & when this statement executes, it will store `0000 0000 0000 0000 0000 0000 0000 0101` data bits in that 4-byte memory location. This was plain & simple. 
- Let's go a bit further, `float f = 3.0;` this statement will also reserve 4 bytes in memory & store data bits in form of 1). the sign bit, 2). exponent & 3). mantissa. Recall how float stored in memory, find more about it here[TODO].
- So this is how compiler stores the value in the variable/object by identifying data-type of `l-value`.
- But when you write like `float f = 3;`, the compiler will confuse that how to store an integer value in float type of memory. So it will automatically presume(Implicit conversion) that you want to store `3.0` rather than `3` which is technically same from the human point of view but it's different when you think from computer memory perspective cause they stored differently.
- There are many such scenarios where you provide data to store in memory which used to represent different data type.
- For example, in the following example, you are trying to assign an object of type `B` into an object of type `A`
```
class A{};
class B {};

int main ()
{
  B b;
  A a = b; 
  return 0;
}
```
- In such scenario compiler can not presume anything & simply throws an compilation error:
```
exit status 1
error: no viable conversion from 'B' to 'A'
  A a = b;
    ^   ~
note: candidate constructor (the implicit copy constructor) not viable: no known conversion from 'B' to 'const A &' for 1st argument
class A{};
      ^
note: candidate constructor (the implicit move constructor) not viable: no known conversion from 'B' to 'A &&' for 1st argument
class A{};
      ^
1 error generated.
```
- But when you define a conversion operator as follows:
```
class B {
public:
  operator A(){
    cout<<"CONVERSION OPERATOR\n";
    return A();
  } 
};
```
- Compiler will simply call this function & wont throw any error because programmer explicitly mentioning that this is how he/she wants to assign.


### C-style casts
```
int main() { 
    float res = 10 / 2;
    cout<<res<<"\n";
    return 0; 
}
```
- When you will try to run the above code, you will get `5` as output which we didn't expect. To initialize `res` variable correctly we need to type cast using float as follows:
```
float res = (float)10 / 2;
```
- Now your answer will be `2.5`. This type of casting is very simple & straight forward as it appears.
- You can also write above casting in C++ as:
```
float res = float(10) / 2;
```
- C-style casts can change a data type without changing the underlying memory representation which may lead to garbage results. 

### `static_cast`
- If you are from C background like me, then this will be your best goto cast. For example:
```
int * p = malloc(10);
```
- When you try to compile above code using C compiler it works fine. But C++ compiler is not kind enough. It will throw an error as follows :
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
- This will work, but this style of cast is not recommended in C++. `static_cast` handles implicit conversions like this. We will primarily use it for converting in places where implicit conversions fail, such as malloc.
```
int * p = static_cast<int*>(malloc(10));
```
- The main advantage of `static_cast` is that it provides compile-time type checking, making it harder to make an inadvertent error. Let's understand this with C++ example:
```
class B {};
class D : public B {};
class X {};

int main()
{
  D* d = new D;
  B* b = static_cast<B*>(d); // this works
  X* x = static_cast<X*>(d); // ERROR - Won't compile
  return 0;
}
```
- As you can see, there is no easy way to distinguish between the two situations without knowing a lot about all the classes involved.
- The second problem is that the C-style casts are too hard to locate. In complex expressions it can be very hard to see C-style casts e.g. `T(something)` syntax is equivalent to `(T)something`.
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
### `const_cast`
- Rather going about description on `const_cast`, why dont we start seeing usecases.
> **1. Not allowed to `const_cast` variables that are actually `const`**
 - You are not allowed to `const_cast` variables that are actually `const`. This results in undefined behavior. `const_cast` is used to remove the const-ness from references and pointers that ultimately refer to something that is not `const`.
- So, this is allowed:
```
int i = 0;
const int& ref = i;
const int* ptr = &i;

const_cast<int&>(ref) = 3;
*const_cast<int*>(ptr) = 3;
```
- It's allowed because `i`, the object being assigned to, is not `const`. The below is not allowed:
```
const int i = 0;
const int& ref = i;
const int* ptr = &i;

const_cast<int&>(ref) = 3;
*const_cast<int*>(ptr) = 3;
```
- Because here `i` is `const` and you are modifying it by assigning it a new value. The code will compile, but its behaviour is undefined (which can mean anything from "it works just fine" to "the program will crash".)
> **2. Modifying data member using `const` `this` pointer**
- `const_cast` can be used to change non-const class members by a method in which this pointer declared as const. - This can also be useful when overloading member functions based on `const`, for instance:
```
class X {
public:
  int var;

  void changeAndPrint(int temp) const
  {
    this->var = temp;   // Throw compilation error
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
> **3. Pass `const` argument to function which accept only non-const argument**
- `const_cast` can also be used to pass const data to a function that doesn’t receive const argument. See following code:
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
> **4. Cast away `volatile` attribute**
- `const_cast` can also be used to cast away `volatile` attribute. Whatever we discussed above in `const_cast` is also valid for `volatile` keyword.

### dynamic cast
- `dynamic_cast` uses the type checking at runtime in contrary to `static_cast` which does it at compile time. `dynamic_cast` is more useful when you don't know the type of input which it represents. Let assume:
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
- As you can see, we dont know which object will be return by `CreateRandom()` at run time but you want to execute `Method1()` of `Derived1` if it returns `Derived1`. So in this scenario you can use `dynamic_cast` as follows
```
Derived1 *pD1 = dynamic_cast<Derived1*>(base);
if(pD1){
  pD1->Method1();
}
```
- In case, if input of `dynamic_cast` does not point to valid data, it will return `nullptr` for pointers or throw a `std::bad_cast` exception for references. In order to work with `dynamic_cast`, your classes must be polymorphic type i.e. must include at least one 'virtual' methods.
- `dynamic_cast` take advantage of `RTTI`(Run Time Type Identification) mechanism.

### `reinterpret_cast`
- `reinterpret_cast` is a compiler directive which tells the compiler to treat the current type as a new type.
- You can use `reinterpret_cast` to cast any pointer or integral type to any other pointer or integral type. 
- This can lead to dangerous situations: nothing will stop you from converting an `int` to an `std::string*`.
- You will use `reinterpret_cast` in your embedded systems. A common scenario where `reinterpret_cast` applies is converting between `uintptr_t` and an actual pointer or between:
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
After reading all this you may confuse on what to use & when! That's why I have created this cheat code 
- Avoid C-style casts. Be sure about what you want while casting.
- Use `static_cast` wherever you were using C-style cast.
- Use `reinterpret_cast` for specific cases where you need to reinterpret underlying data (e.g. converting from a pointer to `uintptr_t`)
- Use `dynamic_cast` wherever you casting pointers and references which points to the inheritance hierarchy. Keep in mind that only use `dynamic_cast` on classes with at least one `virtual` member.
- Use `const_cast` when you need to remove `const` or `volatile` qualifiers. Think carefully before using this cast.

Note: `const_cast` and `reinterpret_cast` should generally be avoided because they are only useful in rare cases and can be harmful if used incorrectly. Avoid `const_cast` and `reinterpret_cast` unless you have a very good reason to use them.

### References
- https://www.learncpp.com/cpp-tutorial/4-4a-explicit-type-conversion-casting/
- https://www.learncpp.com/cpp-tutorial/44-implicit-type-conversion-coercion/
- https://stackoverflow.com/questions/103512/why-use-static-castintx-instead-of-intx
- https://www.geeksforgeeks.org/`const_cast`-in-c-type-casting-operators/
- https://stackoverflow.com/questions/2253168/dynamic-cast-and-static-cast-in-c
