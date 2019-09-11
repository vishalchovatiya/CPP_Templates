### Introduction
- In an earlier article, we have learned about virtual function. In this article, we will learn about the virtual class.
- But I am iterating the same thing as I have mentioned in the earlier article as well. Implementation of a virtual mechanism is purely compiler dependent, no C++ standard is defined for this. Here I am describing the general approach.
- Same as before, before learning anything new we have to see Why it needed?
### Why we need a virtual class?
- When we use inheritance, we basically extending derived class with base class functionality. In simple word, the base class object would be treated as sub-object in the derived class.
- This would create a problem in multiple inheritances if base class sharing the same mutual class as sub-object in the top level hierarchy and you want to access its property. I know this statement is a bit complex. Ok then let see an example.
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
- An instance of `Bottom` will be made up of `Left`, which includes `Top`, and `Right` which also includes `Top`. So we have two sub-object of `Top`. This will create ambiguity as follows:
```
Bottom *bot = new Bottom;
bot->t = 5; // is this Left's t variable or Right's t variable ??
```
- This was by far the simplest reason for the need of virtual base class. Adding more to this, let we consider the following scenarios for the above example:
```
Top   *t_ptr1 = new Left;
Top   *t_ptr2 = new Right; 
```
These both will work fine as `Left` or `Right` object memory layout has `Top` subobject. You can see the memory layout of the `Bottom` object for clear understanding.
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
Now, what happens when we upcast a `Bottom` pointer?
```
Left  *left = new Bottom;
```
This will work fine as Bottom object memory layout starts with `Left` subobject.
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
After this adjustment, we can access the `Bottom` through the `right` pointer as a normal `Right` object.
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
- Virtual inheritance is there to solve this problem. When you specify virtual when inheriting your classes, you're telling the compiler that you only want a single instance.
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
- But interesting question is that How this `bot->t` will be addressed & handle by the compiler? Ok, this is the time to move on next point.

### How virtual class addressing mechanism works
- A class containing one or more virtual base class subobjects, such as `Bottom`, is divided into two regions: 1). invariant region 2). a shared region. 
- Data within the invariant region remains at a fixed offset(which will be decided in compilation step) from the start of the object regardless of subsequent derivations. So members within the invariant region can be accessed directly. In our case, its `Left` & `Right` & `Bottom`.
- The shared region represents the virtual base class subobjects whose location within the shared region fluctuates with an order of derivation & subsequent derivation. So members within the shared region need to be accessed indirectly.
- An invariant region will be placed at the start of objects memory layout and the shared region will be placed at the end. 
- The offset of these shared region objects will be updated in the virtual table. The code necessary for this is augmented by the compiler in the constructor. See below image for reference.
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

### Handling of virtual function in the virtual base class
- Handling of virtual function in virtual base class is the same as we have discussed in our previous article with multiple inheritances. There is nothing special about it.

### Complications of using the virtual base class
> **Downcasting** 
- As we have seen, casting of object Bottom to Right(in other words, upcasting) requires adding offset to a pointer. One might be tempted to think that downcasting can then simply be implemented by subtracting the same offset.
- This process is not easy for the compiler as it seems. To understand this, let us go through an example.
```
class AnotherBottom : public Left, public Right
{
public:
   int ab1;
   int ab2;
};
```
- `Bottom` & `AnotherBottom` have the same inheritance hierarchy except for their own data members. Now consider the following code.
```
Bottom* bottom1 = new Bottom();
AnotherBottom* bottom2 = new AnotherBottom();
Top* top1 = bottom1;
Top* top2 = bottom2;
Left* left = static_cast<Left*>(top1);
```
- Following is memory layout for `Bottom` & `AnotherBottom`
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
- Now consider how to implement the `static_cast` from `top1` to `left`, while taking into account that we do not know whether `top1` is pointing to an object of type `Bottom` or an object of type `AnotherBottom`. It can't be done! The necessary offset depends on the runtime type of `top1` (20 for `Bottom` and 24 for `AnotherBottom`). The compiler will complain:
```
error: cannot convert from a pointer to base class 'Top' to a pointer to derived class 'Left' because the base is virtual
```
- Since we need runtime information, we need to use a dynamic cast instead:
```
Left* left = dynamic_cast<Left*>(top1);
```
- However, the compiler is still unhappy:
```
error: cannot dynamic_cast 'top1' (of type 'class Top*')to type 'class Left*' (source type is not polymorphic)
```
- The problem is that a dynamic cast (as well as the use of `typeid`) needs runtime type information about the object pointed to by `top1`. The compiler did not include that because it did not think that was necessary. To force the compiler to include that, we can add a virtual destructor to `Top`:
```
class Top
{
public:
   virtual ~Top() {} // This line creates magic for us
   int t;
};
```
- Thus, for a downcasting object having virtual base class we need to have at least one virtual function in the virtual base class.

> **Double pointer hack**
- For below code:
```
Bottom* b = new Bottom();
Right* r = b;
```
- We already know that the value of `b` gets adjusted by 8 bytes before it is assigned to `r` so that it points to the `Right` section of the `Bottom` object). Thus, we can legally assign a `Bottom*` to a `Right*`. What about `Bottom**` and `Right**`?
```
Bottom** bb = &b;
Right** rr = bb;
```
- Should the compiler accept this? A quick test will show that the compiler will complain:
```
error: invalid conversion from `Bottom**' to `Right**'
```
- Why? Suppose the compiler would accept the assignment of `bb` to `rr`. We can visualise the result as:
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
- So, `bb` and `rr` both point to `b`, and `b` and `r` point to the appropriate sections of the `Bottom` object. Now consider what happens when we assign to `*rr` (note that the type of `*rr` is `Right*`, so this assignment is valid):
```
*rr = b;    
```
- This is essentially the same assignment as the assignment to `r` above. Thus, the compiler will implement it the same way! In particular, it will adjust the value of `b` by 8 bytes before it assigns it to `*rr`. But `*rr` pointed to `b`! If we visualise the result again:
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
- This is correct as long as we access the `Bottom` object through `*rr`, but as soon as we access it through `b` itself, all memory references will be off by 8 bytes — obviously a very undesirable situation.
- So, in summary, even if `*a` and `*b` are related by some subtyping relation, `**a` and `**b` are not.

> **Constructors of virtual bases**
- The compiler must guarantees that the constructor for all virtual bases of a class gets invoked, and get invoked **only once**. If you don't explicitly call the constructors of your virtual base class (independent of how far up the tree they are), the compiler will automatically insert a call to their default constructors.
- This can lead to some unexpected results. Consider the same class hierarchy again we have been considering so far, extended with constructors:
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
- What would you expect this to output:
```
Bottom bottom(1,2,3,4);
printf("%d %d %d %d %d\n", bottom.Left::a, bottom.Right::a, bottom.b, bottom.c, bottom.d);
```
- You would probably get
```
-1 -1 2 3 4
```
- I know you were expecting different. But if you trace the execution of the constructors, you will find
```
Top::Top()
Left::Left(1,2)
Right::Right(1,3)
Bottom::Bottom(1,2,3,4)
```
- As explained above, the compiler has inserted a call to the default constructor in `Bottom`, before the execution of the other constructors. Then when Left tries to call its base class constructor(`Top`), we find that `Top` has already been initialised and the constructor does not get invoked.
- To avoid this situation, you should explicitly call the constructor of your virtual base(s):
```
Bottom(int _a, int _b, int _c, int _d): Top(_a), Left(_a,_b), Right(_a,_c) 
{ 
   d = _d; 
}
```
> **Pointer Equivalence**
- Once again assuming the same (virtual) class hierarchy, would you expect this to print “Equal”?
```
Bottom* b = new Bottom(); 
Right* r = b;
      
if(r == b)
   printf("Equal!\n");
```
- Bear in mind that the two addresses are not actually equal (`r` is off by 8 bytes). However, that should be completely transparent to the user; so, the compiler actually subtracts the 8 bytes from `r` before comparing it to `b`; thus, the two addresses are considered equal.
- Although, this also stands true for the following code.
```
class base1{};
class base2{};
class derived : public base1, public base2{};

derived *d = new derived();
base2 *b2 = d;

if(b2 == d)
   printf("Equal!\n");
```

### Reference 
- http://www.avabodh.com/cxxin/virtualbase.html
- https://stackoverflow.com/questions/21558/in-c-what-is-a-virtual-base-class
- https://web.archive.org/web/20160413064252/http://www.phpcompiler.org/articles/virtualinheritance.html
- Book: Inside C++ Object Model By Lippman
