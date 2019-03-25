### Intro

### All about `constexpr`

##### Function as `constexpr`
```c++
constexpr int sum(int x, int y) 
{ 
    return x + y; 
} 
  
int main() 
{ 
    const int x = sum(10, 20); 
    cout << x; 
    return 0; 
} 
```
- constexpr specifies that the value of an object or a function can be evaluated at compile time and the expression can be used in other constant expressions. By specifying `constexpr`, we suggest compiler to evaluate function `sum` at compiler time.
- Rules:
1. In C++ 11, a `constexpr` function should contain only one return statement. C++ 14 allows more than one statements.
2. `constexpr` function should refer only constant global variables.
3. `constexpr` function can call only other constexpr function not simple function.
4. Function should not be of void type and some operator like prefix increment (++v) are not allowed in constexpr function.

| Format   | Tag example |
| -------- | ----------- |
| Headings | =heading1=<br>==heading2==<br>===heading3=== |
| New paragraph | A blank line starts a new paragraph |
| Source code block |  ```if (foo) bar else   baz 
sdfsd
sfsd
sdfs``` |
###

### References
