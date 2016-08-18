- **Runtime Polymorphism** in C++ is achived with virtual function. Hence in order to implement the runtime behaviour, C++ compiler inserts Virtual Table(Array of Function pointers) for every class having virtual function or class inherited from the class that has virtual function. 

- Consider following code 
```
#include<iostream>
using namespace std;

class Shape {
   public:
      virtual int area(){
      	cout << "Parent class area :" <<endl;
        return 0;
      }
      void forExample(){
      	cout << "Parent class forExample :" <<endl;
      }
};
class Rectangle: public Shape{
   public:
      int area (){ 
         cout << "Rectangle class area :" <<endl;
         return 0; 
      }
};
```

- If we print sizeof `Shape` class it will give us 4 bytes(8 byte on 64 bit machine). Because it contain pointer to virtual table.
- As the class contains a virtual function C++ compiler inserts a pointer `v_ptr` called as virtual table pointer. 
- Compiler also creates a table for that class called as virtual table(aka vtable) for the class. 
- The table is created at compile time, `v_ptr` holds the address of virtual table each class containing or inherited with atleast one virtual method. 
- Virtual table is a array of function pointers pointing to virtual function.
- In our example, Virtual table of shape class contain pointers to `area` method of shape class. Hence upon calling it calls Shape class area method
```
Shape *s = new Rectangle();
s->area();        //Calls Shape area() without virtual keyword
s->forExample();  //calls Shape's forExample()
```
- As Rectangle Class inherits Shape class, it also inheits data member `v_ptr` of Shape class which will rewriten on new Virtual table created at compile time for Rectangle class. Hence we have separate virtual table for every class having virtual function or class inherited from the class that has virtual function as we said earlier.
- As Rectangle Class has implemented `area` method. Virtual table of Rectangle class containing pointer to `area` method is rewirtten with its own `area` method address while `forExmaple` method still point to parent class i.e. Share class.

```
Shape *s = new Rectangle();
s->area();        //calls Rectangle area() with virtual keyword
s->forExample();  //calls Shape's forExample()
```
