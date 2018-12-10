### How class code synthesized ?
```

```

### How object used in executable function ?
-  Given the following function, where class X defines a copy constructor, virtual destructor, and virtual function foo():
```
X foobar()
{
	X xx;
	X *px = new X;

	// foo() is virtual function
	xx.foo();
	px->foo();
	
	delete px;
	return xx;
};

```
- Probable internal transformation
```
void foobar( X &_result )
{
	// _result replaces local xx & constructor called
	_result.X::X();
	// expand X *px = new X;
	px = _new( sizeof( X ));
	if ( px != 0 )
		px->X::X();
	
	// expand xx.foo(): replaced xx with _result
	foo( &_result );
	// expand px->foo() using virtual mechanism
	( *px->_vtbl[ 2 ] )( px )
	
	// expand delete px;
	if ( px != 0 ) {
		( *px->_vtbl[ 1 ] )( px ); // destructor
		_delete( px );
	}
	// replace named return statement
	// no need to destroy local object xx
	return;
};
```
### How & where constructor code transform/synthesize with inheritance & composition class ?

```
class Foo 
{ 
public: 
  Foo(){cout<<"Foo"<<endl;} 
  ~Foo(){cout<<"~Foo"<<endl;} 
};

class base 
{ 
public: 
  base(){cout<<"base"<<endl;}
  ~base(){cout<<"~base"<<endl;}
};

class Bar /* : public base */
{ 
  Foo foo; 
  char *str; 
public: 
  Bar()
  {
    cout<<"Bar"<<endl;
    str = 0;
  }
  ~Bar(){cout<<"~Bar"<<endl;}
};
```
- Compiler augmented `Bar` constructor would be look like:
```
Bar::Bar()
{
  foo.Foo::Foo(); // augmented compiler code
  
  cout<<"Bar"<<endl; // explicit user code
  str = 0; // explicit user code
}
```
- Same goes for, multiple class member objects requiring constructor initialization. The language specifies that the constructors would be invoked in the order of member declaration within the class. This is accomplished by the compiler.
- If object member does not define a default constructor, a non-trivial default constructor is synthesized by a compiler for respective classes.
- In the case of inheritance, constructor calling sequence is started from base(top-down) to derived manner. Constructor synthesis & augmentation remain same as above. So in above case if you derive `Bar` from `Base` then constructor calling sequence would be `Base` -> `Foo` -> `Bar`.

### How & where destructor code transform/synthesize with inheritance & composition class ?
- In case of destructor, calling sequence is exactly reverse that of a constructor. Like in above case it would be `Bar` -> `Foo` -> `Base`. Synthesis & augmentation remain same as above.

### How & where virtual table inserted in code ?
