### Introduction
- Before learning All about the virtual keyword in C++, I would like to clarify two things
- Implementation of a virtual function is purely compiler dependent, no C++ standard is defined for this.
- Before learning anything we have to see why it needed at first place.
### Why we need a virtual function?
- Let we understand it with an example. Suppose you want to connect to the network or to other mobile using your smartphone.
- You have two choices Bluetooth or Wifi. Although these two are completely different technologies, still some things are common in them at an abstract level like both are communication protocol, both need authentication, etc.
- Let say we have a class of them like as follows:
```
class wifi_t{
	private:
		char _pass[15];
		// storage ...
	public:
		void authenticate();
		void connect();
		// operations ...
};

class bluetooth_t{
	private:
		char _pass[15];
		// storage ...
	public:
		void authenticate();
		void connect();
		// operations ...
};
```
- Now, below is the main application in which you want to connect your device to others.
```
int main()
{
	wifi_t 		*wifi = new wifi_t;
	bluetooth_t 	*bluetooth = new bluetooth_t;

	int pt = selectProtocol();
	
	if(pt == BLUETOOTH){
		bluetooth->authenticate();
		bluetooth->connect();
	}
	else if(pt == WIFI){
		wifi->authenticate();
		wifi->connect();
	}
	return 0;
}
```
- If you observe above code then you will find that despite selecting any protocol some steps are same.
- In this case, you can leverage virtual functionality of C++ as follows:

```
class protocol_t{
	private:
		uint8_t _type;
		// storage ...
	public:
		virtual void authenticate(){};
		virtual void connect(){};
		// operations ...
};

class wifi_t : public protocol_t{
	private:
		char _pass[15];
		// storage ...
	public:
		virtual void authenticate(){};
		virtual void connect(){};
		// operations ...
};

class bluetooth_t : public protocol_t{
	private:
		char _pass[15];
		// storage ...
	public:
		virtual void authenticate(){};
		virtual void connect(){};
		// operations ...
};

void makeConnection(protocol_t *protocol)
{
	protocol->authenticate();
	protocol->connect();
}	

int main()
{
	int pt = selectProtocol();
  
	makeConnection( (pt == WIFI) ? static_cast<protocol_t*>(new wifi_t) : static_cast<protocol_t*>(new bluetooth_t));	

	return 0;
}
```

As you can see there are some benefits we have achieved using virtual keywords are:
1. **Run time polymorphism**: Behavioural functions will be identified at runtime & would be called by their type like if `protocol` is wifi then execute `wifi_t::authenticate()` & `wifi_t::connect()`.
2. **Reusability of code**: Observe `makeConnection` function there is an only single call to behavioural functions we have removed the redundant code from main.
3. **Code would be compact**: Observe earlier `main` function & newer one.

### How virtual function works
![](https://github.com/VisheshPatel/CPP_Templates/blob/master/images/How%20virtual%20function%20works.png)

- When you declare any function virtual, the compiler will transform(augment is the precise word here) some of your code at compile time.
- Like in our case class `protocol_t` will be augmented by a pointer called `vptr` which points to the virtual table.
- This is nothing but an array of a void pointer which includes offset/address of your virtual function. So that it can call your function through that table rather than calling it directly by adding offset to `this` pointer.

So if you call function `authenticate()` using a pointer of `protocol_t` 
```
protocol_t *protocol;
protocol->authenticate();
```
then it would probably be augmented by compiler like this
```
( * protocol->vptr[ 1 ])( protocol ); 
```
Where the following holds:
1. `vptr` represents the internally generated virtual table pointer inserted within each object whose class
declares or inherits one or more virtual functions. (In practice, its name is mangled. There may be
multiple vptrs within a complex class derivation.)
2. `1` in `vptr[ 1 ]` is the index into the virtual table slot associated with `authenticate()`.
3. protocol in its second occurrence represents the `this` pointer.

- When we inherit `protocol_t` class to `wifi_t` class, this virtual table will be literally overridden with its respective overridden/polymorphic function slot. Each virtual function has fixed index in the virtual table, no matter how long inheritance hierarchy is.
- If derived class introduce a new virtual function not present in the base class, the virtual table will be grown by a slot and the address of the function is placed within that slot.

- If you want to summarize virtual keyword functionality in two words then its `indirect calling` of a polymorphic function.

###### FAQ

**Q**. How do we know at runtime that pointer `protocol` will execute a right function(of the object pointed to)?

**A**. In general, we don't know the exact type of the object `protocol` addresses at each invocation of `authenticate()`. We do know, however, that through `protocol` we can access the virtual table associated with the object's class. And the offset of `vptr` is fixed throughout the inheritance hierarchy. Again we also that index of function `authenticate()` in virtual table is fixed throughout the inheritance hierarchy.
This way right `authenticate()` function execution will be guaranteed. 

**Q**. What if there is derived class having more that one base class?

**A**. We will discuss this scenario in the subsequent topic.

**Q**. Where & how this code augment by the compiler?

**A**. We will discuss this in a subsequent topic.

### How pure virtual function works
- When you declare any function as pure virtual, the compiler automatically fills the slot of that pure virtual function with dummy function or so-called place holder `pure_virtual_called()` library instance. And run time exception is placed if somehow this place holder will be called.  
- Rest of calling & virtual table slot mechanism would be the same as a normal virtual function.
### How virtual function works in multiple inheritances.
- Now with multiple inheritance things will get a little bit tricky.
- To understand this behaviour let us take another simplified example as follow :

```
class base1{
  public: 
    int base1_var;

    virtual void base1_func(){}
            
    virtual base1* print(){
      cout<<"print base1\n";
      return this;
    }
};

class base2{
  public: 
    int base2_var;

    virtual void base2_func(){}

    virtual base2* print(){
      cout<<"print base2\n";
      return this;
    }
};

class derived : public base1, public  base2{
  public:
    int derived_var;

    virtual void derived_func(){}

    virtual derived* print(){
      cout<<"print derived\n";
      return this;
    }
};
```
- Here we have derived class with two base classes. In this case, when we declare an object of the derived class, two virtual tables will be created in the derived class object. One for base1 & other for base2. 
- To understand it, first let's assign a base2 pointer the address of a derived class object allocated on the heap:

```
base2 *pb = new derived;
```
- The address of the new derived object must be adjusted to address its base2 subobject. The code to accomplish this is generated at compile time:
```
// transformation to support second base class
derived *temp = new derived;
base2 *pb = temp ? temp + sizeof( base1 ) : 0;
```
- Without this adjustment, any nonpolymorphic use of the pointer would fail, such as
```
// ok even if pbase2 assigned derived object
pb->base2_var;
```
NOTE: There is one exceptional case of virtual destructor which we will discuss in depth in another article


### Reference 
- http://www.avabodh.com/cxxin/virtualbase.html
- 
