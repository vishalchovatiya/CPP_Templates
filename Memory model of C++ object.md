### Bit about this article
- Here I have tried to show you how different objects are stored in memory. We are not going to discuss compiler augmented code, name mangling & working of any C++ mechanism. I have also ignored the stack growth direction.
- It's just all about how different object will be represented in memory.

### Memory layout of C++ object: simple & non-polymorphic 

```
class X
{  
    int x;
    float xx;
    
  public:
    X(){}
    ~X(){}
    
    void printInt(){}
    void printFloat(){}
};
```
- Memory layout: 
```
      |                        |          
      |------------------------| <------ X class object memory layout
      |        int X::x        |
      |------------------------|  stack segment
      |       float X::xx      |       |   
      |------------------------|       |
      |                        |      \|/
      |                        |    
      |                        |  
      |                        |
------|------------------------|----------------
      |         X::X()         | 
      |------------------------|       |   
      |        X::~X()         |       |
      |------------------------|      \|/
      |      X::printInt()     |  text segment
      |------------------------|
      |     X::printFloat()    |
      |------------------------|
      |                        |            
```

- As you can see all data members are in the stack with the same order of their declarations(which is guaranteed by most of the compilers, apparently).
- All other methods, constructor, destructor & compiler augmented code go into the text segment. These methods are then called & passed this pointer implicitly of calling object in its 1st argument which we have discussed in the this[TODO] article.

### Memory layout of C++ object: virtual function & static data member 
```
class X
{  
    int x;
    float xx;
    static int count;
    
  public:
    X(){}
    virtual ~X(){}
    
    virtual void printAll(){}
    void printInt(){}
    void printFloat(){}
    static void printCount(){}
};
```
- Memory layout:
```
      |                        |          
      |------------------------| <------ X class object memory layout
      |        int X::x        |
stack |------------------------|
  |   |       float X::xx      |                      
  |   |------------------------|      |-------|--------------------------|
  |   |         X::_vptr       |------|       |       type_info X        |
 \|/  |------------------------|              |--------------------------|
      |           o            |              |    address of X::~X()    |
      |           o            |              |--------------------------|
      |           o            |              | address of X::printAll() |
      |                        |              |--------------------------|
      |                        |
------|------------------------|------------
      |  static int X::count   |      /|\
      |------------------------|       |
      |           o            |  data segment           
      |           o            |       |
      |                        |      \|/
------|------------------------|------------
      |        X::X()          | 
      |------------------------|       |   
      |        X::~X()         |       |
      |------------------------|       | 
      |      X::printAll()     |      \|/ 
      |------------------------|  text segment
      |      X::printInt()     |
      |------------------------|
      |     X::printFloat()    |
      |------------------------|
      | static X::printCount() |
      |------------------------|
      |                        |
```
- All non-static data members are going into the stack with the same order of their declaration as we have already seen in the previous point.
- Static data member goes into the data segment of memory & it is accessed by scope resolution operator. After compilation, there is nothing like scope & namespace, its just name mangling performed by the compiler, everything will be referred by its address. You can google this to understand clearly.
- Static methods are goes in text segment & are called with scope resolution operator except this pointer is not passed in its argument.
- For virtual keyword, the compiler automatically inserts pointer(`_vptr`) to a virtual table which is used to transform direct function calling in an indirect call(you can see that in this[TODO] article). This virtual table will be created statically in the data segment. Although this depends on compiler implementation.
- In a virtual table, 1st entry points to a `type_info` object which contains information related to current class & DAG(Directed Acyclic Graph) of other base classes if it is derived from them.
- I have not mentioned data type of `_vptr` which also standard does not mention(even I don't know that).

### Memory layout of C++ object: inheritance
```
class X
{  
    int x;
    string str;
    
  public:
    X(){}
    virtual ~X(){}
    
    virtual void printAll(){}
};

class Y : public X
{
    int y;
    
  public:
    Y(){}
    ~Y(){}
    
    void printAll(){}
};
```
- Memory layout:
```
      |                              |          
      |------------------------------| <------ Y class object memory layout
      |          int X::x            |
stack |------------------------------|
  |   |              int string::len |
  |   |string X::str ----------------|
  |   |            char* string::str |         
 \|/  |------------------------------|      |-------|--------------------------|
      |           X::_vptr           |------|       |       type_info Y        |
      |------------------------------|              |--------------------------|
      |          int Y::y            |              |    address of Y::~Y()    |
      |------------------------------|              |--------------------------|
      |               o              |              | address of Y::printAll() |
      |               o              |              |--------------------------|
      |               o              |              
      |                              |              
------|------------------------------|--------
      |           X::X()             | 
      |------------------------------|       |   
      |           X::~X()            |       |
      |------------------------------|       | 
      |         X::printAll()        |      \|/ 
      |------------------------------|  text segment
      |           Y::Y()             |
      |------------------------------|
      |           Y::~Y()            |
      |------------------------------|
      |         Y::printAll()        |
      |------------------------------|
      |      string::string()        |
      |------------------------------|
      |      string::~string()       |
      |------------------------------|
      |      string::length()        |
      |------------------------------|
      |               o              |
      |               o              |
      |               o              |
      |                              |
```
- In the inheritance model, a base class & a data member classes is treated as a subobject of derived class & memory map is created accordingly(as you can see above). 
- All virtual function will be overridden in virtual table & code for this will be generated in constructor of class by compiler. Which we have discussed in our [virtual function series](https://github.com/VisheshPatel/CPP_Templates/blob/master/PART%201:%20All%20about%20virtual%20keyword%20C++:%20How%20virtual%20function%20works%20internally%3F.md).

### Memory layout of C++ object: multiple inheritances & virtual function
```
class X {
  public:
    int x;
    virtual ~X(){}
    virtual void printX(){}
};

class Y {
  public:
    int y;
    virtual ~Y(){}
    virtual void printY(){}
};

class Z : public X, public Y {
  public:
    int z;
    ~Z(){}
    void printX(){}
    void printY(){}
    void printZ(){}
};
```
- Memory layout: 
```
      |                              |          
      |------------------------------| <------ Z class object memory layout
stack |          int X::x            |
  |   |------------------------------|      |-------|--------------------------|
  |   |          X:: _vptr           |------|       |       type_info Z        |
  |   |------------------------------|              |--------------------------|
 \|/  |          int Y::y            |              |    address of Z::~Z()    |
      |------------------------------|              |--------------------------|
      |          Y:: _vptr           |------|       |   address of Z::printX() |
      |------------------------------|      |       |--------------------------|
      |          int Z::z            |      |
      |------------------------------|      |
      |              o               |      |-------|--------------------------|
      |              o               |              |       type_info Z        |
      |              o               |              |--------------------------|
      |                              |              |    address of Z::~Z()    |
------|------------------------------|---------     |--------------------------|
      |           X::~X()            |       |      |   address of Z::printY() |
      |------------------------------|       |      |--------------------------|   
      |          X::printX()         |       |
      |------------------------------|       | 
      |           Y::~Y()            |      \|/ 
      |------------------------------|  text segment
      |          Y::printY()         |
      |------------------------------|
      |           Z::~Z()            |
      |------------------------------|
      |          Z::printX()         |
      |------------------------------|
      |          Z::printY()         |
      |------------------------------|
      |          Z::printZ()         |
      |------------------------------|
      |               o              |
      |               o              |
      |               o              |
      |                              |
```
- In multiple inhertance heirarchy, exact number of virtual table created will be N-1, where N represents number of classes.
- Now, rest of things will be easy to understand for you, i guess.
- Whenever you try to call a method of class Z using any base class pointer, it will be call using respective virtual table. For example:
```
Y *y_ptr = new Z;
y_ptr->printY(); // OK
y_ptr->printZ(); // Not OK, as virtual table of class Y doesn't have address of printZ() method
```
- In above code, `y_ptr` will point to subobject of class Y within complete Z object.
- And call to any method for say `y_ptr->printY();` using `y_ptr` will be resolved like:
```
 ( *y_ptr->_vtbl[ 2 ] )( y_ptr )
```
- You must be wondering why i have passed `y_ptr` as argument here. Its implicit `this` pointer, you can learn about it here[TODO].
### ### Memory layout of C++ object: virtual inheritence
```
class X {int x;};
class Y : public virtual X {int y;};
class Z : public virtual X {int z;};
class A : public Y, public Z {int a;};
```
- Memory layout: 
```
                    |                  |          
 Y class  --------> |------------------| <------ A class object memory layout
sub-object          |    Y::y          |          
                    |------------------|             |------------------| 
                    |    Y::_vptr_Y    |------|      |    offset of X   | // offset starts from Y subobject = 20
 Z class  --------> |------------------|      |----> |------------------|
sub-object          |    Z::z          |             |       .....      |
                    |------------------|             |------------------|  
                    |    Z::_vptr_Z    |------|       
                    |------------------|      |        
 A sub-object ----> |    A::a          |      |      |------------------| 
                    |------------------|      |      |    offset of X   | // offset starts from Z subobject = 12
 X class ---------> |    X::x          |      |----> |------------------|                              
shared sub-object   |------------------|             |       .....      |                                    
                    |                  |             |------------------|                                            
                    |                  |           
```

- Memory representation of derived class having one or more virtual base class is divided into two regions: 
    1. an invariant region
    2. a shared region. 
- Data within the invariant region remains at a fixed offset from the start of the object regardless of subsequent
derivations. 
- But virtual base class memory region is not fixed because it is a shared region & it fluctuates with subsequent derivation & order of derivation. I have discussed more on this in [PART 2: All about virtual keyword C++](https://github.com/VisheshPatel/CPP_Templates/blob/master/PART%202:%20All%20about%20virtual%20keyword%20C++:%20How%20virtual%20class%20works%20internally%3F.md).
