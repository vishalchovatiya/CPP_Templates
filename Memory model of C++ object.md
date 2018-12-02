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

- As you can see all data members are goes into the stack with same order of their declarion(which is gurenteed by most of the compilers, apparently) 
- All other methods, constructor, destructor & compiler augmented code(which i have not shown for simplicity) are goes into text segment. These methods are then called using scope resolution operator & passed this pointer of calling object in its 1st argument explicitly which we discuss in separate article.

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

- All non static data members are goes into the stack with same order of their declarion as we already seen in above point.
- Static data member are goes into the data segment of memory & it is accessed by scope resolution operator.
- Static methods are goes in text segment & are called with scope resolution operator except this pointer is not passed in its argument.
- For virtual keyword, compiler automatically insert pointer(vptr) to virtual table which is used to transfor direct function calling in indirect call(we will see this is separate article). 
- In virtual table, 1st entry points to type_info object which contain information related to current class & DAG(Directed Acyclic Graph) of other base classes if it is derived from them.

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

- In inheritance model, base class is treated as subobject of derived class & memory map is created accordingly(as you can see in above pic). Rest of the things are remain same.

