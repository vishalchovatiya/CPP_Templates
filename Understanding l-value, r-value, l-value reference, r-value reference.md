### Intro
- While I was introducing myself with C++. Most of the time I was struck by compiler saying this is lvalue & that is rvalue kind of things. I was unable to understand until i have googled a bit for the same.
- This is my habit to preserve knowledge in term of an article. So here is a bit about things I have learned so far.
- There is always one thought in my mind before learning anything new: "why I need that?" So let's start with it.
### Why we need lvalue & rvalue kind of jargons?
- If you are using C++ prior to C++11 then you don't need these jargons to write code. But yes it is still useful to understand compilation errors.
- The compiler sees things by expression & to evaluate expression it identify operand & operation. Let's understand this by example:
```
int a = 5;
```
- Here compiler identifies `a` & `5` as operand and `=`(assignment) as operation. Furthermore, compiler divides operand in subcategory named as `rvalues`, `lvalues`, `xvalues`, `glvalues`, and `prvalues` to distinguish between them. This different value types tells the compiler about the source, destination & the scope of information, etc.
- In the above expression `a` is lvalue as it indicates destination memory where rvalue i.e. `5` will be stored.
- When you compile & see above statement in assembly, it would probably look like :
```
...
movl    $5, -4(%ebp)
...
```
- Here, `(%ebp)` is current frame pointer which pull down by `4` bytes which indicates space allocated by compiler for variable `a` in stack. And `movl` instruction store `5` to that memory location directly.
- This is straight forward as long as we are using primitive data types like `int`, `double`, `char`, etc. So compiler will store raw value directly in instruction code itself like in our case its `$5`. After the execution of that instruction, `$5` is not used, so it has expression scope, in other word it is temporary.
- But when we use class & struct which are the user-defined type, things get bit complex & compiler introduce temporary object instead of directly storing the value in instruction code itself.

Bottom Line: we need this kind of jargons to understand compilation error & to see thing from a compilers perspective. And yes! if you are using C++11 or above, you need to understand these jargons to write robust, fast & optimize code.
### What are lvalue & rvalue?
- lvalue & rvalue is compiler identifiers to evaluate expression.
- Any compiler identifier which represent memory location is lvalue. 
- Any compiler identifier which represents data value on the right-hand side of an assignment operator(=) is rvalue. .

> **Examples of lvalue**

- There are two types of lvalue modifiable & non-modifiable(which are const).

**modifiable lvalue**

1. `a = 1;`**:** `a` is lvalue as it represent memory,
2. `int b = a;`**:** `b` & a is lvalue, when `a` is assigned to `b` it becomes an implicit rvalue because a copy of `a` is stored in `b`, not `a` itself,
3. `struct S* ptr = &obj;`**:** `ptr` is lvalue,
4. `arr[20] = 5;`**:** location index 20 in `arr` is lvalue,
6. `int *pi = &i;`**:** `i` is lvalue as it is addressable
5. `*pi = 10;`**:** `*pi` is lvalue as it points to `i`,
6. `class MyClass {}; MyClass X;`**:** `X` is lvalue as it represent memory of user defined type`,
etc.

**non-modifiable lvalue**

1. `const int a=1;`**:** `a` is non-modifiable lvalue,
2. `const int *p=&a;`**:** `p` is non-modifiable lvalue,
etc.

> **Examples of rvalue**
1. `int a = 1;`**:** `1` is rvalue,
2. `int b = a;`**:** `a` is implicit rvalue this case we have already discussed in 2nd point of "Examples of lvalue",
3. `q = p + 5;`**:** valid - `p + 5` is an rvalue,
4. `int result = getInteger();`**:** value returned by `getInteger()` is rvalue,
5. `class cat {}; c = cat();`**:** `cat()` is an rvalue,
etc.
- rvalue could be a function on right hand side of `=` assignment operator which eventually evaluate to object(primitive or user defined).
- rvalues are typically evaluated for their values, have expression scope (they die at the end of the expression they are in) most of the time, and cannot be assigned to. For example:
```
5 = a; // invalid
getInt() = 2; // invalid
```
### lvalue reference
- Now an lvalue reference is a reference that binds to an lvalue. 
- lvalue references are marked with one ampersand `&`.
```
int x = 5;
int &lref = x; // lvalue reference initialized with lvalue x
```
- Prior to C++11, only one type of reference existed in C++, and so it was just called a “reference”. However, in C++11, it’s sometimes called an lvalue reference. 
- lvalue references can only be initialized with modifiable lvalues.
```
const int a = 5;
int &ref = a; // Invalid & error will be thrown by compiler
```
> **Exception**
- We cannot bind lvalue reference to rvalue
```
int &a = 5; // error: lvalue cannot be bound to rvalue 5
```
However, we can bind an rvalue to a const lvalue reference (const reference):
```
const int &a = 5;  // Valid
```
- In this case, compiler convert 5 into lvalue first & then it assign memory location to const reference.
### rvalue reference
- This is by far most useful & bit complex thing you will learn.
- And an rvalue reference is a reference that binds to an rvalue. rvalue references are marked with two ampersands `&&`.
```
int &&rref = 5; // rvalue reference initialized with rvalue 5
```
- rvalues references cannot be initialized with lvalues i.e. 
```
int a = 5;
int &&ref = a; // Invalid & error will be thrown by compiler
```
- rvalue references are more often used as function parameters. This is most useful for function overloads when you want to have different behavior for lvalue and rvalue arguments.
```
void fun(const int &lref) // lvalue arguments will select this function
{
    std::cout << "lvalue reference to const\n";
}
 
void fun(int &&rref) // rvalue arguments will select this function
{
    std::cout << "rvalue reference\n";
}
 
int main()
{
    int x = 5;
    fun(x); // lvalue argument calls lvalue version of function
    fun(5); // rvalue argument calls rvalue version of function
 
    return 0;
}
```
### Why we need rvalue references?
- If you observe the copy constructor & copy assignment operator prototype, it always takes `const` reference object as an argument. Because their primary work is to copy the object. And while copying we don't want to modify the object we have provided on the right-hand side.
- But there are some scenarios where we don't care about the right-hand side object we have provided to copy from. For example:
```
class IntArray{
    int *m_arr;
    int m_len;
public:
    IntArray(int len) : m_len(len), m_arr(new int[len]){}
    ~IntArray(){delete [] m_arr;}

    // Copy Constructor
    IntArray(const IntArray& rhs){
      m_arr = new int[rhs.m_len];
      m_len = rhs.m_len;

      for(int i=0;i<m_len;i++)
        m_arr[i] = rhs.m_arr[i];
    }
};

IntArray func()
{
    IntArray obj(5);    
    // process obj    
    return obj;
}

int main()
{
  IntArray arr = func();   
  return 0;
}

// Note: use "-fno-elide-constructors" option while compiling otherwise it will create copy elision 
```
- By observing this code we conclude that `obj` is not useful after the return of `func` function. But when you return an object by the value it will invoke copy constructor & which will copy all the content from `obj` to `arr`(declared in `main()`) by allocating new resource for `arr`. And when obj goes out of scope it will deallocate its resources.
- Rather than allocating new resources & copying data into it why don't we simply use those existing `obj`'s resources? Let's do that:
```
IntArray(IntArray&& rhs){
    m_arr = rhs.m_arr;
    m_len = rhs.m_len;

    rhs.m_arr = nullptr; // To prevent code crashing 
}
```
- I have just modified copy constructor code as above which accept rvalue reference as an argument rather than lvalue so that our overloaded copy constructor will only be called when there is rvalue is used on the right-hand side. Which simply means this constructor will only be called when right-hand side object is temporary or programmer is no longer care about that object. 
- Implementation simply took ownership of resources from `obj` to `arr` and set right-hand side object's pointer to `NULL` so that it's destructor won't deallocate resource which it is no longer owning.
- In fact, this is move constructor, not a copy constructor. Whose primary task is to take/move ownership of resources.
- Consider following move constructor prototype for more solid understanding:
```
IntArray(IntArray&& rhs)
{
    ...
}
```
- The message of this code is this: "The object that `rhs` binds to is YOURS. Do whatever you like with it, no one will care anyway." It's a bit like giving a copy to `IntArray` but without making a copy.
- This can be interesting for two purposes: 
    1. improving performance (as we are not allocating new resources & transferring content).
    2. taking over ownership (since the object the reference binds to has been abandoned by the caller).

- I know you might be thinking that why just we don't modify copy constructor by removing `const` keyword from it. Let's do that as well
```
IntArray(IntArray& rhs){
}
```
- Compilation error
```
exit status 1
error: no matching constructor for initialization of 'IntArray'
  IntArray arr = func();
           ^     ~~~~~~
note: candidate constructor not viable: expects an lvalue for 1st argument
    IntArray(IntArray& rhs){
    ^
1 error generated.
```
- If you see the `note` our overloaded copy constructor asking for lvalue. What we are doing is providing rvalue. As when we return an object by value, temporary(which falls under rvalue category) object will be created and supplied to our copy constructor. And as we have already seen above lvalue reference cannot bind to rvalue object.
- Don't think about changing your copy constructor's argument as `const` lvalue reference, I know we have studied that its exception & we can bind `const` lvalue reference to rvalue/temporary object. But in that case, you can not move/transfer resource as it is `const`.

### References
- https://www.fluentcpp.com/2018/02/06/understanding-lvalues-rvalues-and-their-references/
- https://www.learncpp.com/cpp-tutorial/15-2-rvalue-references/
- https://www.geeksforgeeks.org/lvalue-and-rvalue-in-c-language/
- https://www.bogotobogo.com/cplusplus/C11/4_C11_Rvalue_Lvalue.php
