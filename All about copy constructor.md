### Conditions: Where copy constructor is used?
1. Object initialization by another object of same class/type.
```
class X { ... };
X x;

// Copy constructor will be called
X xx = x;
```
2. When an object is passed as an argument to a function
```
extern void foo( X x );
void bar()
{
  X xx;

  // Copy constructor will be called
  foo( xx );

  // ...
}
```
3. When a function returns a class object
```
X foo_bar()
{
  X xx;
  // ...;

  // Copy constructor will be called
  return xx;
}
```

### How: Bitwise copy & memberwise copy

```
#include "Word.h"

Word noun( "block" );

void foo()
{
  Word verb = noun;
  // ...
}
```
- If class `word` defined as follows, then it **exhibits bitwise copy constructor** as the initialization of verb need not result in a function call.
```
class Word {
  public:
    Word( const char* );
    ~Word() { delete [] str; }
    // ...
  private:
    int cnt;
    char *str;
};
```
- But, if class `word` defined as follows, then it **exhibits memberwise copy constructor** & result in a function call becuase `string` declares explicit copy coonstructor.
```
class Word {
  public:
    Word( const string& );
    ~Word();
    // ...
  private:
    int cnt;
    string str;
};
```
- In this case, the compiler needs to synthesize a copy constructor in order to invoke the copy constructor of the member class `string` object:
```
// A synthesized copy constructor
inline Word::Word( const Word& wd )
{
  str.string::string( wd.str );
  cnt = wd.cnt;
}
```
### Not: Bitwise Copy Semantics!
When are bitwise copy semantics not exhibited by a class? There are four instances:
1. When the class contains a member object of a class for which a copy constructor exists (either
explicitly declared by the class designer, as in the case of the previous `string` class, or synthesized by
the compiler, as in the case of class `Word`)
2. When the class is derived from a base class for which a copy constructor exists (again, either explicitly
declared or synthesized)
3. When the class declares one or more virtual functions
4. When the class is derived from an inheritance chain in which one or more base classes are virtual
- In instances 1 and 2, the implementation needs to insert invocations of the member or base class copy
constructors inside the synthesized copy constructor. 

### Other ways: can copy constructor be invoked
```
  X x0;
  X x1( x0 );
  X x2 = x0;
  X x3 = x( x0 );
  // ...
```
- All these statements transform into invocation of the copy constructor.
```
X x0;
X x1;
X x2;
X x3;
// compiler inserted invocations
// of copy constructor for X
x1.X::X( x0 );
x2.X::X( x0 );
x3.X::X( x0 );
// ...
```
