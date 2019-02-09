### Introduction
- In earlier article we have learned about virtual function. In this article we will learn about virtual class.
- But i am iterating same thing as i have mentioned in earlier article as well. Implementation of a virtual mehcanism is purely compiler dependent, no C++ standard is defined for this. Here i am describing general approach.
- Same as before, before learning anything new we have to see why it needed at first place.
### Why we need a virtual class?
- When we use inheritance, we basically extending derived class wiht base class functionality. In simple word, base class object would be treated as sub-object in derived class.
- This would create a problem in multiple inheritance, if base class sharing same mutual class as sub-object in top level hierarchy. I know this statement is complex. Ok then let see example.
```
class Top {public: int t; };
class Left : public Top {public: int l; };
class Right : public Top {public: int r; };
class Bottom : public Left, public Right {public: int b; };
```
- The above class hierarchy/inheritance results in the "diamond" which looks like this:
```
    Top
   /   \
Left   Right
   \   /
   Bottom
```
There are two reasons for the need of virtual base class:
##### Reason 1
- An instance of `Bottom` will be made up of `Left`, which includes `Top`, and `Right` which also includes `Top`. So we have two sub-object of `Top`. This will create abiguity as follows:
```
Bottom *bot = new Bottom;
bot->t = 5; // is this Left's t variable or Right's t variable ??
```
##### Reason 2
```
|                      |
|----------------------|  <------ Bottom bot;   // Bottom object 
|    Left::Top::t      |
|----------------------|
|    Left::l           |
|----------------------|
|    Right::Top::t     |
|----------------------|
|    Right::r          |
|----------------------|
|    Bottom::b         |
|----------------------|
|                      |
|                      |

```

Let we consider following scenarios
```
Top   *t_ptr1 = new Left;
Top   *t_ptr2 = new Right; 
```
These both will work fine as Left or Right object memory layout start with Top subobject. You can see the memory layout of `Bottom` object above for clear understanding.

Now what happens when we upcast a `Bottom` pointer?
```
Left  *left = new Bottom;
```
This will work fine as Bottom object memory layout start with `Left` subobject.
However, what happens when we upcast to Right?
```
Right  *right = new Bottom;
```
For this to work, we have to adjust the `right` pointer value to make it point to the corresponding section of the `Bottom` layout:
```
|                      |
|----------------------|
|    Left::Top::t      |
|----------------------|
|    Left::l           |
|----------------------|  <------ right;
|    Right::Top::t     |
|----------------------|
|    Right::r          |
|----------------------|
|    Bottom::b         |
|----------------------|
|                      |
|                      |
```
After this adjustment, we can access Bottom through the Right pointer as a normal `Right` object; however, `Bottom` and `Right` now point to different memory locations. For completeness' sake, consider what would happen when we do
```
Top* Top = Bottom;
```
Right, nothing at all. This statement is ambiguous: the compiler will complain
```
error: `Top' is an ambiguous base of `Bottom'
```
The two possibilities can be disambiguated using
```
Top* topL = (Left*) Bottom;
Top* topR = (Right*) Bottom;
```
After these two assignments, `topL` and `Left` will point to the same address, as will `topR` and `Right`.
##### Solution 
- Virtual inheritance is there to solve these problem. When you specify virtual when inheriting your classes, you're telling the compiler that you only want a single instance.
```
class Top {public: int t; };
class Left : virtual Top {public: int l; };
class Right : virtual Top {public: int r; };
class Bottom : public Left, public Right {public: int b; };
```
- This means that there is only one "instance" of Top included in the hierarchy. Hence
```
Bottom *bot = new Bottom;
bot->t = 5; // no longer ambiguous
```
- This may seem more obvious and simpler from a programmer's point of view, from the compiler's point of view, this is vastly more complicated. Co
- But interesting question is that How this `bot.t` will be addressed & handle by compiler ? Ok, this is the time to move on next point.

### How virtual class addressing mechanism works
- A class containing one or more virtual base class subobjects, such as `Bottom`, is divided into two regions: 1). invariant region 2). a shared region. 
- Data within the invariant region remains at a fixed offset(which will be decided in compilation step) from the start of the object regardless of subsequent derivations. So members within the invariant region can be accessed directly. 
- The shared region represents the virtual base class subobjects whose location within the shared region fluctuates with order of derivation & subsequent derivation. So members within the shared region need to be accessed indirectly.
- In this case, objects in invariant region will be placed at start in order of inheritance & objects in shared region will be placed at the end. The offset of these  shared region objects will be updated in virtual table by the compiler augmented code. See below image for reference.
```
|                        |          
|------------------------| <------ Bottom bot;   // Bottom object           
|    Left::l             |          
|------------------------|                     |----------------------| 
|    Left::_vptr_Left    |----------|          |    offset of Top     | // offset starts from left subobject = 20
|------------------------|          |----------|----------------------|
|    Right::r            |                     |      ...             |
|------------------------|                     |----------------------|  
|    Right::_vptr_Right  |----------|           
|------------------------|          |            
|    Bottom::b           |          |          |----------------------| 
|------------------------|          |          |    offset of Top     | // offset starts from right subobject = 12
|    Top::t              |          |----------|----------------------|                                                 
|------------------------|                     |      ...             |                                                  
|                        |                     |----------------------|                                            
|                        |                
```
- Now come to our interesting question "How this `bot->t` will be addressed ?"
```
Bottom *bot = new Bottom;
bot->t = 5; // no longer ambiguous
```
Above code `bot->t` will be probably transformed into
```
(this + _vptr_Left[-1])->t = 5;
```

### Handling of virtual function in virtual base class
- 

### Special handling cases
The initialization of one class object with another in which there is a virtual base class subobject also invalidates bitwise copy semantics.

### Reference 
- http://www.avabodh.com/cxxin/virtualbase.html
- https://stackoverflow.com/questions/21558/in-c-what-is-a-virtual-base-class
- 
