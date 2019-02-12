## How different object stores in memory ?

### Simple object model representation of below class

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

### Object with virtual & static keyword model representation of below class
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

### Inheritence object model representation of below class
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
![](https://github.com/VisheshPatel/CPP_Templates/blob/master/images/Inheritence%20object%20model%20representation.png)

- In the inheritance model, a base class is treated as a subobject of derived class & memory map is created accordingly(as you can see in above pic). Rest of the things are remaining the same.

### Multiple inheritence object model representation of below classes
```
class Point2d {
  public:
    // ...
  protected:
    float _x, _y;
};

class Vertex {
  public:
    // ...
  protected:
    Vertex *next;
};

class Vertex2d : public Point2d, public Vertex {
  public:
    //...
  protected:
    float mumble;
};
```
![](Insert image HERE)
- Page 70 of 182
### Multiple inheritence with virtual keyword object model representation of below classes

```
class Point {
public:
virtual ~Point();
virtual Point& mult( float ) = 0;
// ... other operations ...
float x() const { return _x; }
virtual float y() const { return 0; }
virtual float z() const { return 0; }
// ...
protected:
Point( float x = 0.0 );
float _x;
};
class Point2d : public Point {
public:
Point2d( float x = 0.0, float y = 0.0 )
: Point( x ), _y( y ) {}
~Point2d();
// overridden base class virtual functions
Point2d& mult( float );
float y() const { return _y; }
// ... other operations ...
protected:
float _y;
};
class Point3d: public Point2d {
public:
Point3d( float x = 0.0,
float y = 0.0, float z = 0.0 )
: Point2d( x, y ), _z( z ) {}
~Point3d();
// overridden base class virtual functions
Point3d& mult( float );
float z() const { return _z; }
// ... other operations ...
protected:
float _z;
};
```


### Virtual inheritence object model representation of below classes
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
