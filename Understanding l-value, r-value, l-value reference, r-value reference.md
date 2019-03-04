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
Now an lvalue reference is a reference that binds to an lvalue. lvalue references are marked with one ampersand (&).
And an rvalue reference is a reference that binds to an rvalue. rvalue references are marked with two ampersands (&&).
