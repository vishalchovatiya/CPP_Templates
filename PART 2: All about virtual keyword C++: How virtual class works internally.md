### Introduction
- In earlier article we have learned about virtual function. In this article we will learn about virtual class.
- But i am iterating same thing as i have mentioned in earlier article as well. Implementation of a virtual mehcanism is purely compiler dependent, no C++ standard is defined for this. Here i am describing general approach.
- Same as before, before learning anything new we have to see why it needed at first place.
### Why we need a virtual class?
- When we use inheritance, we basically extending derived class wiht base class functionality. In simple word, base class object would be treated as sub-object in derived class.
- This would create a problem in multiple inheritance, if base class sharing same mutual class as sub-object in top level hierarchy and you want to access its property. I know this statement is bit complex. Ok then let see example.
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
- An instance of `Bottom` will be made up of `Left`, which includes `Top`, and `Right` which also includes `Top`. So we have two sub-object of `Top`. This will create abiguity as follows:
```
Bottom *bot = new Bottom;
bot->t = 5; // is this Left's t variable or Right's t variable ??
```
- This was by far simplest reason for the need of virtual base class. Adding more to this, let we consider following scenarios for above example:
```
Top   *t_ptr1 = new Left;
Top   *t_ptr2 = new Right; 
```
These both will work fine as `Left` or `Right` object memory layout has `Top` subobject. You can see the memory layout of `Bottom` object for clear understanding.
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
```
Now what happens when we upcast a `Bottom` pointer?
```
Left  *left = new Bottom;
```
This will work fine as Bottom object memory layout start with `Left` subobject.
However, what happens when we upcast to `Right`?
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
After this adjustment, we can access `Bottom` through the `right` pointer as a normal `Right` object.
But, what would happen when we do
```
Top* Top = new Bottom;
```
This statement is ambiguous: the compiler will complain
```
error: `Top' is an ambiguous base of `Bottom'
```
Although you can use force typecasting as follows:
```
Top* topL = (Left*) Bottom;
Top* topR = (Right*) Bottom;
```

##### Solution 
- Virtual inheritance is there to solve these problem. When you specify virtual when inheriting your classes, you're telling the compiler that you only want a single instance.
```
class Top {public: int t; };
class Left : virtual public Top {public: int l; };
class Right : virtual public Top {public: int r; };
class Bottom : public Left, public Right {public: int b; };
```
- This means that there is only one "instance" of `Top` included in the hierarchy. Hence
```
Bottom *bot = new Bottom;
bot->t = 5; // no longer ambiguous
```
- This may seem more obvious and simpler from a programmer's point of view, from the compiler's point of view, this is vastly more complicated. 
- But interesting question is that How this `bot->t` will be addressed & handle by compiler ? Ok, this is the time to move on next point.

### How virtual class addressing mechanism works
- A class containing one or more virtual base class subobjects, such as `Bottom`, is divided into two regions: 1). invariant region 2). a shared region. 
- Data within the invariant region remains at a fixed offset(which will be decided in compilation step) from the start of the object regardless of subsequent derivations. So members within the invariant region can be accessed directly. In our case, its `Left` & `Right` & `Bottom`.
- The shared region represents the virtual base class subobjects whose location within the shared region fluctuates with order of derivation & subsequent derivation. So members within the shared region need to be accessed indirectly.
- An invariant region will be placed at start of objects memory layout and shared region will be placed at the end. 
- The offset of these shared region objects will be updated in virtual table. Code necessary for this is augmented by compiler in constructor. See below image for reference.
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
Above code will probably be transformed into
```
Bottom *bot = new Bottom;
(bot + _vptr_Left[-1])->t = 5;
```

### Handling of virtual function in virtual base class
- 

### Complication of using virtual base class
> **Downcasting** 
- As we have seen in Reason 2, casting of object Bottom to Right(in other words, upcasting) requires adding offset to this pointer. One might be tempted to think that downcasting (going the other way) can then simply be implemented by subtracting the same offset.
- This process is not easy for compiler as it seems. To understand this, let we go through example.
Suppose we extend our inheritance hierarchy with the following class.
```
class AnotherBottom : public Left, public Right
{
public:
   int ab1;
   int ab2;
};
```
- `Bottom` & `AnotherBottom` have same inhertance heirarchy except their data members. Now consider the following code.
```
Bottom* bottom1 = new Bottom();
AnotherBottom* bottom2 = new AnotherBottom();
Top* top1 = bottom1;
Top* top2 = bottom2;
Left* left = static_cast<Left*>(top1);
```
Following is memory layout for `Bottom` & `AnotherBottom`
```
         |                        |                     |                        |
         |------------------------|<------ Bottom       |------------------------|<------ AnotherBottom
         |    Left::l             |                     |    Left::l             |
         |------------------------|                     |------------------------|
         |    Left::_vptr_Left    |                     |    Left::_vptr_Left    |
         |------------------------|                     |------------------------|
         |    Right::r            |                     |    Right::r            |
         |------------------------|                     |------------------------|
         |    Right::_vptr_Right  |                     |    Right::_vptr_Right  |
         |------------------------|                     |------------------------|
         |    Bottom::b           |                     |    AnotherBottom::ab1  |
top1---->|------------------------|                     |------------------------|
         |    Top::t              |                     |    AnotherBottom::ab2  |
         |------------------------|           top2----->|------------------------|  
         |                        |                     |    Top::t              |
                                                        |------------------------|
                                                        |                        |
```
- Here we do not know whether top1 is pointing to an object of type Bottom or an object of type AnotherBottom. It can't be done! The necessary offset depends on the runtime type of top1 (20 for Bottom and 24 for AnotherBottom). The compiler will complain:
```
main.cpp:19:39: error: cannot convert from pointer to base class 'Top' to pointer to derived class 'Left' because the base is virtual
   Left* left = static_cast<Left*>(top1);
```
- Since we need runtime information, we need to use a dynamic cast instead:
```
Left* left = dynamic_cast<Left*>(top1);
```
- However, the compiler is still unhappy:
```
main.cpp:19:40: error: cannot dynamic_cast 'top1' (of type 'class Top*')to type 'class Left*' (source type is not polymorphic)
   Left* left = dynamic_cast<Left*>(top1);
```
- The problem is that a dynamic cast (as well as use of typeid) needs runtime type information about the object pointed to by top1. However, if you look at the diagram, you will see that all we have at the location pointed to by top1 is an integer (a). The compiler did not include a vptr.Top because it did not think that was necessary. To force the compiler to include this vptr, we can add a virtual destructor to Top:
```
class Top
{
public:
   virtual ~Top() {} // This line creates magic for us
   int t;
};
```
- This change necessitates a vptr for Top. 
### Double pointer hack
- This is were it gets slightly confusing, although it is rather obvious when you give it some thought. We consider an example. Assume the class hierarchy presented in the last section (Downcasting). We have seen previously what the effect is of
```
Bottom* b = new Bottom();
Right* r = b;
```
(the value of b gets adjusted by 8 bytes before it is assigned to r, so that it points to the Right section of the Bottom object). Thus, we can legally assign a Bottom* to a Right*. What about Bottom** and Right**?
```
Bottom** bb = &b;
Right** rr = bb;
```
Should the compiler accept this? A quick test will show that the compiler will complain:
```
error: invalid conversion from `Bottom**' to `Right**'
```
Why? Suppose the compiler would accept the assignment of bb to rr. We can visualise the result as:
```
    |--------------| --------> |-------------|           |                        | 
    |      bb      |           |      b      | --------> |------------------------|<------ Bottom 
    |--------------|    /----> |-------------|           |    Left::l             |              
                       /                                 |------------------------|              
                      /                                  |    Left::_vptr_Left    |              
    |--------------| /         |-------------| --------> |------------------------|              
    |      rr      |           |      r      |           |    Right::r            |              
    |--------------|           |-------------|           |------------------------|              
                                                         |    Right::_vptr_Right  |              
                                                         |------------------------|              
                                                         |    Bottom::b           |              
                                                         |------------------------|              
                                                         |    Top::t              |              
                                                         |------------------------|           
                                                         |                        |             
```
- So, bb and rr both point to b, and b and r point to the appropriate sections of the Bottom object. Now consider what happens when we assign to *rr (note that the type of *rr is Right*, so this assignment is valid):
```
*rr = b;	
```
This is essentially the same assignment as the assignment to r above. Thus, the compiler will implement it the same way! In particular, it will adjust the value of b by 8 bytes before it assigns it to *rr. But *rr pointed to b! If we visualise the result again:
```
    |--------------| --------> |-------------|           |                        | 
    |      bb      |           |      b      |           |------------------------|<------ Bottom 
    |--------------|    /----> |-------------|\          |    Left::l             |              
                       /                       \         |------------------------|              
                      /                         \        |    Left::_vptr_Left    |              
    |--------------| /         |-------------|---\-----> |------------------------|              
    |      rr      |           |      r      |           |    Right::r            |              
    |--------------|           |-------------|           |------------------------|              
                                                         |    Right::_vptr_Right  |              
                                                         |------------------------|              
                                                         |    Bottom::b           |              
                                                         |------------------------|              
                                                         |    Top::t              |              
                                                         |------------------------|           
                                                         |                        |  
```
This is correct as long as we access the Bottom object through *rr, but as soon as we access it through b itself, all memory references will be off by 8 bytes — obviously a very undesirable situation.

So, in summary, even if `*a` and `*b` are related by some subtyping relation, `**a` and `**b` are not.

### Constructors of Virtual Bases
The compiler must guarantee that all virtual pointers of an object are properly initialised. In particular, it guarantees that the constructor for all virtual bases of a class get invoked, and get invoked only once. If you don't explicitly call the constructors of your virtual superclasses (independent of how far up the tree they are), the compiler will automatically insert a call to their default constructors.

This can lead to some unexpected results. Consider the same class hierarchy again we have been considering so far, extended with constructors:
```
class Top
{
public:
   Top() { a = -1; } 
   Top(int _a) { a = _a; } 
   int a;
};

class Left : virtual public Top
{
public:
   Left() { b = -2; }
   Left(int _a, int _b) : Top(_a) { b = _b; }
   int b;
};

class Right : virtual public Top
{
public:
   Right() { c = -3; }
   Right(int _a, int _c) : Top(_a) { c = _c; }
   int c;
};

class Bottom : public Left, public Right
{
public:
   Bottom() { d = -4; } 
   Bottom(int _a, int _b, int _c, int _d) : Left(_a, _b), Right(_a, _c) 
	{ 
      d = _d; 
	}
   int d;
};
```

What would you expect this to output:
```
Bottom bottom(1,2,3,4);
printf("%d %d %d %d %d\n", bottom.Left::a, bottom.Right::a, 
   bottom.b, bottom.c, bottom.d);
```
You would probably get
```
-1 -1 2 3 4
```
Why? If you trace the execution of the constructors, you will find
```
Top::Top()
Left::Left(1,2)
Right::Right(1,3)
Bottom::Bottom(1,2,3,4)
```
As explained above, the compiler has inserted a call to the default constructor in Bottom, before the execution of the other constructors. Then when Left tries to call its superconstructor (Top), we find that Top has already been initialised and the constructor does not get invoked.

To avoid this situation, you should explicitly call the constructor of your virtual base(s):
```
Bottom(int _a, int _b, int _c, int _d): Top(_a), Left(_a,_b), Right(_a,_c) 
{ 
   d = _d; 
}
```

### Pointer Equivalence
Once again assuming the same (virtual) class hierarchy, would you expect this to print “Equal”?
```
Bottom* b = new Bottom(); 
Right* r = b;
      
if(r == b)
   printf("Equal!\n");
```
Bear in mind that the two addresses are not actually equal (r is off by 8 bytes). However, that should be completely transparent to the user; so, the compiler actually subtracts the 8 bytes from r before comparing it to b; thus, the two addresses are considered equal.
### Casting to `void*`
Finally, we consider what happens we can cast an object to void*. The compiler must guarantee that a pointer cast to void* points to the “top” of the object. Using the vtable, this is actually very easy to implement. You may have been wondering what the offset to top field is. It is the offset from the vptr to the top of the object. So, a cast to void* can be implemented using a single lookup in the vtable. Make sure to use a dynamic cast, however, thus:
```
dynamic_cast<void*>(b);
```
### Special handling cases
The initialization of one class object with another in which there is a virtual base class subobject also invalidates bitwise copy semantics.

### Reference 
- http://www.avabodh.com/cxxin/virtualbase.html
- https://stackoverflow.com/questions/21558/in-c-what-is-a-virtual-base-class
- https://web.archive.org/web/20160413064252/http://www.phpcompiler.org/articles/virtualinheritance.html
