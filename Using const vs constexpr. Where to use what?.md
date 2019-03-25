### Intro

### All about `constexpr`

##### Function as `constexpr`
```c++
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

constexpr int sum(int x, int y)
{
    return x + y;
}

int main()
{
    int result = sum(10, 20);
    cout << result;
    return 0;
}
```
- constexpr specifies that the value of an object or a function can be evaluated at compile time and the expression can be used in other constant expressions. 
```
+--------------------------------------+--------------------------------------+
|       int result = sum(10, 20);      |    const int result = sum(10, 20);   |
+--------------------------------------+--------------------------------------+
|  1   main:                           |      main:                           |
|  2   ....                            |      ....                            |
|  3   ....                            |      ....                            |
|  4   ....                            |      ....                            |
|  5           subl    $20, %esp       |      	      subl    $20, %esp       |
|  6           subl    $8, %esp        |              movl    $30, -12(%ebp)  |
|  7           pushl   $20             |              subl    $8, %esp        |
|  8           pushl   $10             |              pushl   $30             |
|  9           call    _Z3sumii        |              pushl   $_ZSt4cout      |
|  10          addl    $16, %esp       |              call    _ZNSolsEi       |
|  11          movl    %eax, -12(%ebp) |      ....                            |
|  12          subl    $8, %esp        |      ....                            |
|  13          pushl   -12(%ebp)       |      ....                            |
|  14          pushl   $_ZSt4cout      |                                      |
|  15          call    _ZNSolsEi       |                                      |
|  16  ....                            |                                      |
|  17  ....                            |                                      |
|  18  ....                            |                                      |
+--------------------------------------+--------------------------------------+
```
By specifying `constexpr`, we suggest compiler to evaluate function `sum` at compiler time.
- Rules:
1. In C++ 11, a `constexpr` function should contain only one return statement. C++ 14 allows more than one statements.
2. `constexpr` function should refer only constant global variables.
3. `constexpr` function can call only other constexpr function not simple function.
4. Function should not be of void type and some operator like prefix increment (++v) are not allowed in constexpr function.

##### `constexpr` with constructors

### `constexpr` vs `const`
- They serve different purposes. constexpr is mainly for optimization while const is for practically const objects like value of Pi.
Both of them can be applied to member methods. Member methods are made const to make sure that there are no accidental changes by the method. On the other hand, the idea of using constexpr is to compute expressions at compile time so that time can be saved when code is run.
- const can only be used with non-static member function whereas constexpr can be used with with member and non-member functions, even with constructors but with condition that argument and return type must be of literal types.

### References
