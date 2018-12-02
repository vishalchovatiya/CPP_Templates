### Conditions where copy constructor is used
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

### Bitwise copy & memberwise copy

```
#include "Word.h"

Word noun( "block" );

void foo()
{
  Word verb = noun;
  // ...
}
```
- If class `word` declaration as follows, then it **exhibits bitwise copy constructor** as the initialization of verb need not result in a function call.
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
- But, if class `word` declaration as follows, then it **exhibits memberwise copy constructor** becuase string declares explicit copy coonstructor.
```
class Word {
  public:
    Word( const String& );
    ~Word();
    // ...
  private:
    int cnt;
    String str;
};
```
- where String declares an explicit copy constructor:
```
class String {
  public:
    String( const char * );
    String( const String& );
    ~String();
    // ...
};

```
- In this case, the compiler needs to synthesize a copy constructor in order to invoke the copy constructor of the member class String object:
```
// A synthesized copy constructor
inline Word::Word( const Word& wd )
{
  str.String::String( wd.str );
  cnt = wd.cnt;
}
```
### Bitwise Copy Semantics—Not!
When are bitwise copy semantics not exhibited by a class? There are four instances:
1. When the class contains a member object of a class for which a copy constructor exists (either
explicitly declared by the class designer, as in the case of the previous String class, or synthesized by
the compiler, as in the case of class Word)
2. When the class is derived from a base class for which a copy constructor exists (again, either explicitly
declared or synthesized)
3. When the class declares one or more virtual functions
4. When the class is derived from an inheritance chain in which one or more base classes are virtual
- In instances 1 and 2, the implementation needs to insert invocations of the member or base class copy
constructors inside the synthesized copy constructor. 
