##### `typedef`
| C/C++ | C++11|
| :---: | :---: |
|`typedef double new_t;`   | `using new_t = double;`    |

##### Initialization
| C/C++ | C++11|
| :---: | :---: |
| `int a = 2;`| `int a(2);` or `int a{2};` |


##### `auto`
| C/C++ | C++11|
| :---: | :---: |
|`auto` = local variables|`auto` = automatically deduce the type based on data|
| `int add(int x, int y);` | `auto add(int x, int y) -> int;` |

| C++11 | C++14|
| :---: | :---: |
|`auto` can not use in function argument & return type|`auto` can be use in function return type|

- C++
```
struct Rectangle
{
	double length;
	double width;
};  
```
- C++11/14
```
struct Rectangle
{
	double length = 1.0; // non-static member initialization
	double width = 1.0;
};
```
