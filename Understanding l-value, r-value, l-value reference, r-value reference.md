### Intro
- While i was introducing myself with C++. Most of the time i was struck by compiler saying this is l-value & that is r-value kind of things. I was unable to uderstand util i have googled a bit for the same.
- This is my habit to preserve the knowledge in term of article. So here is bit about things i have learned so far.
- There is always one thought in my mind before learning anything new: "why i need that?" So lets start with it.
### Why we need l-value & r-value kind of jargons?
- If you are using C++ prior to C++11 then you dont need this jargons to write code. But yes it is still useful to understand compilation errors.
- Compiler sees things by expression & to evaluate expression it identify operand & operation. Let's understand this by example:
```
int a = 5;
```
- Here compiler identifies `a` & `5` as operand & `=`(assignment) as operation. Further more compiler devides operand in subcatagory named as rvalues, lvalues, xvalues, glvalues, and prvalues to distinguish between them. These different value tells compiler about source of information, destination to store result, scope of information, etc.
- In above expression `a` is l-value as it indicates destination memory where r-value i.e. `5` will be stored.
- When you compile & see above statement in assembly, it would probably look like :
```...
movl    $5, -4(%ebp)
...
```
- Here, `(%ebp)` is current frame pointer which pull down by `4` bytes which indicates space allocated by compiler for variable `a` in stack. And `movl` instruction store `5` to that memory location directly.
- This is straight forward as long as we are using premitive data types like int, dobule, char, etc. So compiler will store raw value directly instruction code itself like in our case its `$5`. After the execution of that instruction, `$5` is not used, so it has expression scope, in other word it is temporary.
- But when we use class & struct which are user defined type, things get bit complex & compiler introduce temporary object instead of directly storing value in instruction code itself.

Bottom Line: we need this kind of jargons to understand compilation error & to see thing from compilers perspective. And yes! if you are using C++11 or above it, you need to understand this jargons to write robust, fast & optimize code.
### What are l-value & r-value?
- l-value & r-value is compiler identifiers to evalute expression.
- Any compiler identifier which points-to/represent memory location is l-value. 
- Any compiler identifier which represent data value on right hand side of an assignment operator(=) is r-value. .

> **Examples of l-value**

- There are two types of l-value modifiable & non-modifiable(which are const).

**modifiable l-value**

1. `a = 1; // a is l-value as it represent memory of primitive data type`,
2. `int b = a; // b & a is l-value, when a is assigned to b it becomes an implicit rvalue because a copy of a is stored in b, not a itself`,
3. `struct S* ptr = &obj; // ptr is l-value`,
4. `arr[20] = 5; // location index 20 in arr is l-value`,
6. `int *pi = &i;  // i is l-value as it is addressable`
5. `*pi = 10; // *pi is l-value`,
6. `class cat {}; cat c;   // c is l-value as it represent memory of user defined type`
etc.

**non-modifiable l-value**

1. `const int a=1; // a is non-modifiable l-value`,
2. `const int *p=&a; // p is non-modifiable l-value`,
etc.

> **Examples of r-value**
1. `int a = 1; // 1 is r-value`,
2. `int b = a; // a is implicit r-value this case we have already discussed in 2nd point of "Examples of l-value"`,
3. `q = p + 5; // valid - "p + 5" is an r-value `,
4. `int result = getInteger(); // value returned by getInteger() is r-value`,
5. `class cat {}; c = cat();   // cat() is an rvalue`,
etc.
- r-value could be a function on right hand side of `=` assignment operator which eventually evaluate to object(primitive or user defined) which in turn is data value.
- r-values are typically evaluated for their values, have expression scope (they die at the end of the expression they are in) most of the time, and cannot be assigned to. For example:
```
5 = a; // invalid
getInt() = 2; // invalid
```

```
a = b; // a & b are of same type, defined somewhere else
```
- In above assignment we dont want `b` to be change. This non-assignment rule makes sense, because assigning a value applies a side-effect to the object. 
- Since r-values have expression scope, if we were to assign a value to an r-value, then the r-value would either go out of scope before we had a chance to use the assigned value in the next expression (which makes the assignment useless) or we’d have to use a variable with a side effect applied more than once in an expression (which by now you should know causes undefined behavior!).

### l-value reference
- Now an lvalue reference is a reference that binds to an lvalue. 
- lvalue references are marked with one ampersand `&`.
```
int x = 5;
int &lref = x; // l-value reference initialized with l-value x
```
- Prior to C++11, only one type of reference existed in C++, and so it was just called a “reference”. However, in C++11, it’s sometimes called an l-value reference. 
- l-value references can only be initialized with modifiable l-values.
```
const int a = 5;
int &ref = a; // Invalid & error will be thrown by compiler
```
> **Exception**
- We cannot bind lvalue reference to rvalue
```
int &a = 5; // error: lvalue cannot be bound to rvalue 7
```
However, we can bind an rvalue to a const lvalue reference (const reference):
```
const int &a = 5;  // Valid
```
- In this case compiler convert 5 into lvalue first & then it assign memory location to const reference.
### r-value reference
- This is by far most usefull & bit complex thing you will learn.
- And an rvalue reference is a reference that binds to an rvalue. rvalue references are marked with two ampersands `&&`.
```
int &&rref = 5; // r-value reference initialized with r-value 5
```
- r-values references cannot be initialized with l-values i.e. 
```
int a = 5;
int &&ref = a; // Invalid & error will be thrown by compiler
```
- r-value references are more often used as function parameters. This is most useful for function overloads when you want to have different behavior for l-value and r-value arguments.
```
void fun(const int &lref) // l-value arguments will select this function
{
	std::cout << "l-value reference to const\n";
}
 
void fun(int &&rref) // r-value arguments will select this function
{
	std::cout << "r-value reference\n";
}
 
int main()
{
	int x = 5;
	fun(x); // l-value argument calls l-value version of function
	fun(5); // r-value argument calls r-value version of function
 
	return 0;
}
```
### Why we need r-value references?
- If you observe copy constructor & copy assignment operator prototype, it always takes `const` reference object as arguemtn. Because their primary work is to copy the object. And while copying we dont want to modify our r-value as we have already seen in above `a = b` example.
- But there are some scenarios where we dont care about the r-value or object we have provided to copy from. For example:
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
- By observing this code we conclude that `obj` is not useful after return of `func` function. But when you return object by value it will invoke copy constructor & which will copy all the content from `obj` to `arr` in `main` function by acquiring new resource.
- Rather than initializing new resources & copying data to it why dont we just simply use `obj`'s resources? Because `obj` is anyway goind out of scope after return of `func` function. Let's do that:
```
IntArray(IntArray&& rhs){
	m_arr = rhs.m_arr;
	m_len = rhs.m_len;

	rhs.m_arr = nullptr; // To prevent destructor from crashing code
}
```
- I have just modified copy constructor code as above which accept r-value reference rather than l-value, implementation simply took ownership of resources from `obj` to `arr`. Infact this is move constructor not copy constructor. Whose primary task to take ownership.
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

- I know you might be thingking that why just we dont modify copy constructor by remving `const` keyword from it. Let's do that as well
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
note: candidate constructor not viable: expects an l-value for 1st argument
    IntArray(IntArray& rhs){
    ^
1 error generated.
```
- If you see the `note` our overloaded copy constructor asking for l-value. What we are doing is providing r-value. As when we return object by value, temporary(anonymous) object will be created and supplied to our copy constructor which falls under r-value category as it is temporary object.
- As we have discussed earlier we can only bind l-values to l-value referece.

### References
- https://www.fluentcpp.com/2018/02/06/understanding-lvalues-rvalues-and-their-references/
- https://www.learncpp.com/cpp-tutorial/15-2-rvalue-references/
- https://www.geeksforgeeks.org/lvalue-and-rvalue-in-c-language/
- https://www.bogotobogo.com/cplusplus/C11/4_C11_Rvalue_Lvalue.php
