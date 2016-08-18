
### Characteristics of Abstract Class
- Abstract class cannot be instantiated, but pointers and refrences of Abstract class type can be created.
- Classes inheriting an Abstract Class must implement all pure virtual functions, or else they will become Abstract too.


### Why can't we create Object of Abstract Class ?
- When we create a pure virtual function in class, we reserve a slot for a function in the VTABLE, but doesn't put any address in that slot(remember we use '= 0' for pure virtual function). Hence the VTABLE will be incomplete.
- As the VTABLE for Abstract class is incomplete, compiler will not let the creation of object for such class and will display an errror message whenever you try to do so.
- This is same as you write c program without main & compile it. Compiler gives you error "undefined reference to main".It reserve a slot for a function, but doesn't put any address in that slot so compiler doesnt let you create binary file(output).
