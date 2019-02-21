
| C | C++ | C++11 | C++14 | C++17 |
| :---: | :---: | :---: | :---: | :---: |
| `typedef double new_t;`  |   `typedef double new_t;`   | `using new_t = double;`    | | |
| `int a = 2;`| `int a(2);` or `int a{2};` ||||
||`auto` = local variables|`auto` = automatically deduce the type based on data|||
|||`auto` can not use in function argument & return type|`auto` can be use in function return type||
|  | `int add(int x, int y);` | `auto add(int x, int y) -> int;` | | |
| C | ```struct Rectangle{<br>double length;<br>double width;<br>};``` | ```struct Rectangle { 	double length = 1.0; // non-static member initialization  	double width = 1.0; };``` | C++14 | C++17 |
| C | C++ | C++11 | C++14 | C++17 |
| first line<br>second line|||||
