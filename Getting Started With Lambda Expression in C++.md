### Hello World with lambda expression
```
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	/*
	 * [] -> Capture Specification
	 * Below is function declaration where func is function pointer
	 * Auto return type deduction -> void
	 */
	auto func = [] () { cout << "Hello world" << endl;
						std::cout << __PRETTY_FUNCTION__ << std::endl;
					  };
	func();
	return 0;
}
```

### Simple lambda expression
```
#include <iostream>
#include <algorithm>

/*********************************************************************/

void display(int a)
{
    std::cout << a << " ";
}

int main(int argc, char *argv[]) {
	int arr[] = { 1, 2, 3, 4, 5 };
	std::for_each(arr, arr + sizeof(arr) / sizeof(int), &display);
	std::cout << std::endl;
 
}

/*********************************************************************/
/*
int main(int argc, char *argv[]) {
	int arr[] = { 1, 2, 3, 4, 5 };
	std::for_each(arr, arr + sizeof(arr) / sizeof(int),
		[](int a) {
			std::cout << a << " ";  
		});
	std::cout << std::endl;
 
}
*/
/*********************************************************************/
```

### Capture variables
```
#include <iostream>

int main(int argc, char const *argv[])
{
	int x = 100, y = 200;
	/*
	 * Demonstrate: 1. [=] 2. [&] 3. [x] 4. [&x] 5. [=,&y] 6. [&y,x]
	 */
	auto func = []() { 
		x = x + 1;
		///y = y + 1; 
		std::cout << x << "," << y << std::endl;
		std::cout << "Function Type : " << __PRETTY_FUNCTION__ << std::endl;
		};

	func();
	std::cout << x << "," << y << std::endl;
	return 0;
}
```

### Lambda as parameter
```
#include <iostream>
#include <functional>

/*
 * Demostrate using void f(std::function<int (int)>)
 */
template<typename Functor>
void f(Functor functor)
{

   std::cout << "Function Type : " << __PRETTY_FUNCTION__ << std::endl;
   std::cout << functor(10) << std::endl;
}

void f2(std::function<int (int)> functor)
{
   std::cout << "Function Type : " << __PRETTY_FUNCTION__ << std::endl;
   std::cout << functor(10) << std::endl;
}
int g(int x)
{
    return x * x;
}

int main() 
{
    auto lambda_func = [=] (int x) { std::cout << x * 50 << std::endl; return x * 100; };
    f(lambda_func); //pass lambda
    f(g);      //pass function
}
```

### `this` pointer
```
#include <iostream>

class Demo
{
public:
    Demo () : _x( 3 ) {}
    void func ()
    {
    	/*
    	 * this pointer can be captured using [this], [=] or [&]
    	 * In any of the above cases class member variable(including private)
    	 * can be used as it is.
    	 */
        [_x] () { std::cout << _x << std::endl; _x = 100; } ();
    }

private:
        int _x;
};

int main()
{
    Demo demo;
    demo.func();
    demo.func();
}
```
