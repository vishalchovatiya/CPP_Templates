### Intro
### Why we need l-value & r-value kind of jargons?


- l-value & r-value is compiler identifiers to evalute expression
There are two types of l-value modifiable & non-modifiable(which are const).
Any compiler identifier points to memory location is l-value. 
r-value = identifier which represent data value on right hand side of an assignment operator(=).
r-value could be function which eventually represet object which in turn is data representation.
r-values are typically evaluated for their values, have expression scope (they die at the end of the expression they are in), and cannot be assigned to.
This non-assignment rule makes sense, because assigning a value applies a side-effect to the object. Since r-values have expression scope, if we were to assign a value to an r-value, then the r-value would either go out of scope before we had a chance to use the assigned value in the next expression (which makes the assignment useless) or we’d have to use a variable with a side effect applied more than once in an expression (which by now you should know causes undefined behavior!).
l-value = locator value = identifier which represents memory location. For example, variable name, *ptr where ptr points to memory location, etc.
r-value = value which is not l-value = value appear on right hand side of assignment(=) operator. Like int a = 5; // 5 = r-value, q = p + 5; // p + 5 is r-value, etc.

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
