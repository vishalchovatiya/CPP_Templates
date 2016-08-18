

### Why can't we create Object of Abstract Class ?
- When we create a pure virtual function in class, we reserve a slot for a function in the VTABLE, but doesn't put any address in that slot. Hence the VTABLE will be incomplete.
- As the VTABLE for Abstract class is incomplete, compiler will not let the creation of object for such class and will display an errror message whenever you try to do so.
- This is same as you write c program without main & compile it. Compiler gives you error "undefined reference to main".It reserve a slot for a function, but doesn't put any address in that slot so compiler doesnt let you create binary file(output).
