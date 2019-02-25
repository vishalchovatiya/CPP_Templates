#### `typedef`
| C/C++ | C++11|
| :---: | :---: |
|`typedef double new_t;`   | `using new_t = double;`    |
|`bool (*fcnPtr)(int, int)`|`using fcnPtr = bool (*)(int, int)`|
#### Initialization
| C/C++ | C++11|
| :---: | :---: |
| `int a = 2;`| `int a(2);` or `int a{2};` |

- C++
```
// 1. Static array initialization
int arr[2];
arr[0] = 9;
arr[1] = 7;

// 2. Dynamic array initialization
int *array = new int[2];
array[0] = 1;
array[1] = 2;
```
- C++11
```
// 1. Static array initialization
int arr[2] = {1, 2}; // Note `=` operator

// 2. Dynamic array initialization
int *array = new int[2] {1, 2}; // Note no `=` operator
```

#### `for` loop
- C++
```
for (int i = 0; i < arrSize; ++i)
    var = arr[i];
```
- C++11
```
// range-based for loop
for(int element : arr)	
    var = element;

// Can also use auto keyword & reference here efficiently
for(auto &element : arr)	
    var = element;    
```

#### `auto`
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
