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

