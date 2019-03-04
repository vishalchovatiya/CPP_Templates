
### Intro
### Why we need move constructors and move assignment ?
### Why we add double `&&` i.e. `class(class &&rhs)` in move constructor ?
### Why we do not add `const` in move constructor ?
### When are the move constructor and move assignment called?
### `std::move`
Most of the time, when you try to use move semantics, but the objects you have to work with are l-values, not r-values.
### Scenarios where you can use move semantics
- swap function
- vector
### Things you have to consider while using move semantics
- smart pointers should never be dynamically allocated themselves (otherwise, there is the risk that the smart pointer may not be properly deallocated, which means the object it owns would not be deallocated, causing a memory leak).
- 

### Referrence
