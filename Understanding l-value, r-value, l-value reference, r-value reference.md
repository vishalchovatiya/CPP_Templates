### Intro
### Why we need l-value & r-value kind of jargons?
- If you are using C++ earlier than C++11 then you dont need this jargons to write code. But yes it is still useful to understand compilation error.
- Compiler sees things by expression & to evaluate expression it identify operand & operation. Let's understand this by example:
```
int a = 5;
```
- Here compiler identifies `a` & `5` as operand & `=`(assignment) as operation. Further mode compiler devides operand in rvalues, lvalues, xvalues, glvalues, and prvalues to distinguish between them. These different value tells compiler about source of information, destination where to store result, scope of information, etc.
- In above expression `a` is l-value as it indicates destination memory where r-value i.e. `5` will be stored.
- When you compile & see above statement in assembly, it would probably look like :
```
...
movl    $5, -4(%ebp)
...
```
- Here, `(%ebp)` is current frame pointer which pull down by `4` bytes which indicates space allocated by compiler for variable `a` in stack. And `movl` instruction store `5` to that memory location directly.
- This is straight forward as long as we are using premitive data types like int, dobule, char, etc. But when we use class & struct which are user defined type, things get bit complex & compiler introduce temporary object instead of directly storing `5` to memory location.
### What are l-value & r-value?
- l-value & r-value is compiler identifiers to evalute expression.
- Any compiler identifier which points-to/represent memory location is l-value. 
- Any compiler identifier which represent data value on right hand side of an assignment operator(=) is r-value. .

> **Examples of l-value**
There are two types of l-value modifiable & non-modifiable(which are const).
**modifiable l-value**
1. `a = 1; // a is l-value`
2. `int b = a; // b is l-value`
3. `struct S* ptr = &obj; // ptr is l-value`
4. `arr[20] = 5; // location index 20 in arr is l-value`
5. ``
6. ``

> **Examples of r-value**
1. `int a = 1; // 1 is r-value`
2. `int b = a; // a is r-value`
3. `q = p + 5; // valid - "p + 5" is an r-value `
4. ``
5. ``
6. ``
- r-value could be function which eventually represet object which in turn is data representation.
- r-values are typically evaluated for their values, have expression scope (they die at the end of the expression they are in), and cannot be assigned to. For example:
```
int a, b = 5;
a = b
```
- In above assignment we dont want `b` to be change. This non-assignment rule makes sense, because assigning a value applies a side-effect to the object. 
- Since r-values have expression scope, if we were to assign a value to an r-value, then the r-value would either go out of scope before we had a chance to use the assigned value in the next expression (which makes the assignment useless) or we’d have to use a variable with a side effect applied more than once in an expression (which by now you should know causes undefined behavior!).

### l-value reference
Now an lvalue reference is a reference that binds to an lvalue. lvalue references are marked with one ampersand (&).

Prior to C++11, only one type of reference existed in C++, and so it was just called a “reference”. However, in C++11, it’s sometimes called an l-value reference. L-value references can only be initialized with modifiable l-values.

### r-value reference
And an rvalue reference is a reference that binds to an rvalue. rvalue references are marked with two ampersands (&&).
```
int x = 5;
int &lref = x; // l-value reference initialized with l-value x
int &&rref = 5; // r-value reference initialized with r-value 5
```

R-values references cannot be initialized with l-values.
- R-value references are more often used as function parameters. This is most useful for function overloads when you want to have different behavior for l-value and r-value arguments.
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
Why we need r-value reference?
unlike l-value reference r-value reference is used to take ownership of object.
Consider following code to understand this more solid:

```
void f(MyClass&& x)
{
    ...
}
```
The message of this code to f is this: “The object that x binds to is YOURS. Do whatever you like with it, no one will care anyway.” It’s a bit like giving a copy to f… but without making a copy.
This can be interesting for two purposes: improving performance (see move constructors below) and taking over ownership (since the object the reference binds to has been abandoned by the caller.

Note that this could not be achieved with lvalue references. For example this function:
```
void f(MyClass& x)
{
    ...
}
```
can modify the value of the object that x binds to, but since it is an lvalue reference, it means that somebody probably cares about it at call site.
I mentioned that lvalue const references could bind to rvalues:

```
void f(MyClass const& x)
{
    ...
}
```
but they are const, so even though they can bind to a temporary unnamed object that no one cares about, f can’t modify it.
Note that we cannot pass an lvalue to f, because an rvalue reference cannot bind to an lvalue. The following code:


MyClass x;
f(x);
1
2
MyClass x;
f(x);
triggers this compilation error:


error: cannot bind rvalue reference of type 'MyClass&&' to lvalue of type 'MyClass'
f(x);
   ^
There is a way to call f with our lvalue x: by casting it explicitly into an rvalue reference. This is what std::move does:


MyClass x;
f(std::move(x));

MyClass x;
f(std::move(x));
So when you std::move an lvalue, you need to be sure you won’t use it any more, because it will be considered like a disposable object by the rest of the code.
