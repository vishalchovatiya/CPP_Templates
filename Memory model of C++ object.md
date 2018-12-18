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
![](https://github.com/VisheshPatel/CPP_Templates/blob/master/images/simple%20object%20model.png)

- As you can see all data members are going into the stack with the same order of their declarations(which is guaranteed by most of the compilers, apparently) 
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
![](https://github.com/VisheshPatel/CPP_Templates/blob/master/images/Object%20with%20virtual%20%26%20static%20keyword%20model.png)

- All non-static data members are going into the stack with the same order of their declaration as we already seen in above point.
- Static data member goes into the data segment of memory & it is accessed by scope resolution operator.
- Static methods are goes in text segment & are called with scope resolution operator except this pointer is not passed in its argument.
- For virtual keyword, the compiler automatically inserts pointer(vptr) to a virtual table which is used to transform direct function calling in an indirect call(we will see this is a separate article). 
- In a virtual table, 1st entry points to a type_info object which contain information related to current class & DAG(Directed Acyclic Graph) of other base classes if it is derived from them.

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
