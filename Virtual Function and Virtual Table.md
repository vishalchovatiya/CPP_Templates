

- **Runtime Polymorphism** in C++ is achived with virtual function. Hence in order to implement the runtime behaviour C++ compiler inserts Virtual Table(Array of Function pointer) for every class having virtual function or class inherited from the class that has virtual function. 

- Consider following code 
```
#include<iostream>
using namespace std;

class Shape {
   public:
      virtual int area()
      {
         cout << "Parent class area :" <<endl;
         return 0;
      }
};
class Rectangle: public Shape{
   public:
      int area ()
      { 
         cout << "Rectangle class area :" <<endl;
         return 0; 
      }
};
```

- If we print sizeof `Shape` class it will give us 4 bytes(8 byte on 64 bit machine). Because it contain pointer to virtual table.
- As the class contains a virtual function C++ compiler inserts a pointer v_ptr called as virtual table pointer. 
- Compiler also creates a table for that class called as virtual table(aka vtable) for the class. 
- The table is created at compile time, v_ptr holds the address of virtual table each class containing or inherited with atleast one virtual method. 
- Virtual table is a array of function pointers pointing to virtual function.

virtual table contains pointers to `area` method of shape class. Hence 
```
Shape *s = new Shape();
s->area();    //Calls Shape area() method
```

- As Rectangle Class inherits Shape class, & as we know Shape class has a data member v_ptr. 
- Rectangle class inheits v_ptr of Shape class but new Virtual table will be created compile time for Rectangle class. 
- Hence v_ptr of Rectangle class holds the address of virtual table of Rectangle class. 
- As Rectangle class has implemented `area` method. virtual table of Rectangle class contains pointer to `area` method of Rectangle class but pointer to `area` method of base class i.e Shape class.

```
Shape *s = new Rectangle();
s->Area();    //calls Truck ApplyBreak
```
