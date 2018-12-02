## How different object stores in memory ?

### Simple object model representation of below class stores in memory

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
![Complete CAN Frame with Electrical Signals- Source:wikipedia](images/CAN-Bus-frame_in_base_format_without_stuffbits.svg)

### Object with virtual & static keyword model representation of below class stores in memory
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
![Complete CAN Frame with Electrical Signals- Source:wikipedia](images/CAN-Bus-frame_in_base_format_without_stuffbits.svg)

### Inheritence object model representation of below class stores in memory
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

