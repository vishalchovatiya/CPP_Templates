### How class code synthesized ?
```
class X
{  
    int x;
};
```
- Compiler augmentation of above class would be as follows:
```
class X
{  
    int x;
public:
    X(){}
    ~X(){}
    X(const X &rhs){}
    opeator=(const X &rhs){}
    
};
```
- TODO above

### How object used in executable function ?
-  Given the following function, where class X defines a copy constructor, virtual destructor, & virtual function foo():
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

### How & where destructor code transform/synthesize with inheritance & composition class ?

### How & where virtual table inserted in code ?
