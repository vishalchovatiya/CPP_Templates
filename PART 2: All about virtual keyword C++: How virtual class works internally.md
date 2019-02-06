### Introduction
- In earlier article we have learned about virtual function. In this article we will learn about virtual class.
- But i am iterating same thing as i have mentioned in earlier article as well. Implementation of a virtual mehcanism is purely compiler dependent, no C++ standard is defined for this. Here i am describing general approach.
- Same as before, before learning anything new we have to see why it needed at first place.
### Why we need a virtual class?
- When we use inheritance, we basically extending derived class wiht base class functionality. In simple word, base class would be treated as sub-class in derived class.
- Object of derived class also contain data members of base class. This way derived class object also contain base class **sub-object** within.
- Ya! i know i have said same thing in all above sentences.
- This would create a problem in multiple inheritance, if base class sharing same mutual class as sub-object in top level hierarchy. I know this statement is complex. Ok then let see example.
```
class top {public: int t; };
class left : public top {public: int l; };
class right : public top {public: int r; };
class bottom : public left, public right {public: int b; };
```
- The above class hierarchy/inheritance results in the "diamond" which looks like this:
```
    top
   /   \
left   right
   \   /
   bottom
```
- An instance of `bottom` will be made up of `left`, which includes `top`, and `right` which also includes `top`. So we have two sub-object of `top`. This will create abiguity as follows:
```
bottom bot;
bot.t = 5; // is this left's t variable or right's t variable ??
```
- Virtual inheritance is there to solve this problem. When you specify virtual when inheriting your classes, you're telling the compiler that you only want a single instance.
```
class top {public: int t; };
class left : virtual top {public: int l; };
class right : virtual top {public: int r; };
class bottom : public left, public right {public: int b; };
```
- This means that there is only one "instance" of top included in the hierarchy. Hence
```
bottom bot;
bot.t = 5; // no longer ambiguous
```
- But interesting question is that How this `bot.t` will be addressed & handle by compiler ? Ok, this is the time to move on next point.

### How virtual class addressing mechanism works
- Test

### Special handling cases
The initialization of one class object with another in which there is a virtual base class subobject also invalidates bitwise copy semantics.

### Reference 
- http://www.avabodh.com/cxxin/virtualbase.html
- https://stackoverflow.com/questions/21558/in-c-what-is-a-virtual-base-class
- 

### Introduction
- In earlier article we have learned about virtual function. In this article we will learn about virtual class.
- But i am iterating same thing as i have mentioned in earlier article as well. Implementation of a virtual mehcanism is purely compiler dependent, no C++ standard is defined for this. Here i am describing general approach.
- Same as before, before learning anything new we have to see why it needed at first place.
### Why we need a virtual class?
- When we use inheritance, we basically extending derived class wiht base class functionality. In simple word, base class would be treated as sub-class in derived class.
- Object of derived class also contain data members of base class. This way derived class object also contain base class **sub-object** within.
- Ya! i know i have said same thing in all above sentences.
- This would create a problem in multiple inheritance, if base class sharing same mutual class as sub-object in top level hierarchy. I know this statement is complex. Ok then let see example.
```
class A { public: int var; };
class B : public A {};
class C : public A {};
class D : public B, public C {};
```
- The above class hierarchy/inheritance results in the "diamond" which looks like this:
```
  A
 / \
B   C
 \ /
  D
```
- An instance of D will be made up of B, which includes A, and C which also includes A. So we have two sub-object of A. This will create abiguity as follows:
```
D d;
d.var = 5; // is this B's var or C's var ??
```
- Virtual inheritance is there to solve this problem. When you specify virtual when inheriting your classes, you're telling the compiler that you only want a single instance.
```
class A { public: int var; };
class B : public virtual A {};
class C : public virtual A {};
class D : public B, public C {};
```
- This means that there is only one "instance" of A included in the hierarchy. Hence
```
D d;
d.var = 5; // no longer ambiguous
```
- But interesting question is that How this `d.var` will be addressed & handle by compiler ? Ok, this is the time to move on next point.

### How virtual class addressing mechanism works
- Test

### Special handling cases
The initialization of one class object with another in which there is a virtual base class subobject also invalidates bitwise copy semantics.

### Reference 
- http://www.avabodh.com/cxxin/virtualbase.html
- https://stackoverflow.com/questions/21558/in-c-what-is-a-virtual-base-class
- 

