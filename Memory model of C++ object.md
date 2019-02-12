## How different object stores in memory ?

### Memory layout of simple non-polymorphic class

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

- As you can see all data members are going into the stack with the same order of their declarations(which is guaranteed by most of the compilers, apparently).
- All other methods, constructor, destructor & compiler augmented code(which I have not shown for simplicity) go into the text segment. These methods are then called using scope resolution operator & passed this pointer(not shown here for simplicity) of calling object in its 1st argument explicitly which we discuss in the separate article.

### Memory layout of class having virtual function & static data member 
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
- All non-static data members are going into the stack with the same order of their declaration as we already seen in above point.
- Static data member goes into the data segment of memory & it is accessed by scope resolution operator.
- Static methods are goes in text segment & are called with scope resolution operator except this pointer is not passed in its argument.
- For virtual keyword, the compiler automatically inserts pointer(vptr) to a virtual table which is used to transform direct function calling in an indirect call(we will see this is a separate article). This virtual table will be created in data segment only.
- In a virtual table, 1st entry points to a type_info object which contain information related to current class & DAG(Directed Acyclic Graph) of other base classes if it is derived from them.
- Although, compiler performs name mangling which i have not shown for simplicity.
- I have not mentioned data type of `vptr` which also standard does not mention(even i dont know that).

### Memory layout of class with inheritence
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
      |           Y::_vptr           |------|       |       type_info Y        |
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
- All virtual function will be overridden in virtual table & code for this will generated in constructor of class by compiler. Which we have discussed in our [virtual function series](https://github.com/VisheshPatel/CPP_Templates/blob/master/PART%201:%20All%20about%20virtual%20keyword%20C++:%20How%20virtual%20function%20works%20internally%3F.md).

### Memory layout of class having multiple inheritence with virtual function
```
class X {
  public:
    int x;
    virtual ~X(){//...}
    virtual void printX(){//...}
};

class Y {
  public:
    int y;
    virtual ~Y(){//...}
    virtual void printY(){//...}
};

class Z : public X, public Y {
  public:
    int z;
    ~Z(){//...}
    void printX(){//...}
    void printY(){//...}
    void printZ(){//...}
};
```
### Memory layout of class having virtual inheritence
```
class X {};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};
```
![](Insert image HERE)
- Memory representation of derived class having one or more virtual base class is divided into two regions: 1). an invariant region & 2). a shared region. Data within the invariant region remains at a fixed offset from the start of the object regardless of subsequent
derivations. But base class memory region is not fixed because it is shared region & it fluctuates with each derivation. We will see the access of shared region in separate article.
- Page 74 of 182
