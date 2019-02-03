### Introduction
- Before learning All about virtual keyword in C++, i would like to clear two things 
1. Implementation of virtual function is purely compiler dependent, no C++ standard is defined for this.
2. Before learning anything we have to see why it needed at first place.

### Why we need virtual function ?
Let we understand it with example.
Suppose you want to connect to network or to other mobile using your smart phone.
You have two choices bluetooth or wifi.
Althought these two are completely different technologies but come things are common in them at abstract level like both are communication protocol, both need authentication, etc.
Let say we have class of them like as follows:
```
class wifi_t{
	private:
		// storage ...
	public:
		void authenticate();
		void connect();
		...
};

class bluetooth_t{
	private:
		// storage ...
	public:
		void authenticate();
		void connect();
		...
};
```
Let say now you have main application where you want to connect your device to others
```
int main()
{
	wifi_t 			  *wifi = new wifi_t;
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
If you observe above code then you will find that despite of selecting any protocol some steps are same.
In this case you can leaverage virtual functionality of C++ as follows:

```
class protocol_t{
	private:
		// storage ...
	public:
		virtual void authenticate(){};
		virtual void connect(){};
    // ...
};

class wifi_t : public protocol_t{
	private:
		// storage ...
	public:
		virtual void authenticate(){};
		virtual void connect(){};
		// ...
};

class bluetooth_t : public protocol_t{
	private:
		// storage ...
	public:
		virtual void authenticate(){};
		virtual void connect(){};
		// ...
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

As you can see there are some benefits we have achieved using virtual keywords are 
1. **Run time polymorphism**: Behavioural functions will be identified at runtime & would be called by their type like if `protocol` is wifi then execute `wifi_t::authenticate()` & `wifi_t::connect()`.
2. **Reusability of code**: Observe `makeConnection` function there is only single call to behavioural functions we have removed redundant code from main.
3. **Code would be compact**: Observe earlier `main` function & newer one.

### How virtual function works
When you declare any function virtual, compiler will transform(augment is precise word here) some of your code at compile time.
If you want to summurize virtual keyword functionality in two words then its `indirect calling` of function.

Like in our case class protocol_t will be augmented by a pointer called vptr which points to virtual table.
This is nothing but a array of void pointer which includes offset of your virtual function. So that it can call your function through that table rather than calling it directly by adding offset to this pointer.

So if you call function `authenticate()` using pointer of protocol_t 

protocol_t *protocol;
protocol->authenticate();

then it would probably be augmented by compiler like this

( * protocol->vptr[ 1 ])( protocol ); 

where the following holds:
1. `vptr` represents the internally generated virtual table pointer inserted within each object whose class
declares or inherits one or more virtual functions. (In practice, its name is mangled. There may be
multiple vptrs within a complex class derivation.)
2. `1` in `vptr[ 1 ]` is the index into the virtual table slot associated with `authenticate()`.
3. protocol in its second occurrence represents the `this` pointer.

When we inherit `protocol_t` class to `wifi_t` class, this virtual table will be literally overridden with its respective overridded/polymorphic function slot. Each virtual function has fixed index in virtual table, no matter how long inheritance heirarchy is.
If derived class introduce a new virtual function not present in the base class, the virtual table will be grown by a slot and the address of the function is placed within that slot.

FAQ
Q. How do we know at runtime that pointer `protocol` will execute right function(of object pointed to)?
A. In general, we don't know the exact type of the object `protocol` addresses at each invocation of `authenticate()`. 
We do know, however, that through `protocol` we can access the virtual table associated with the object's class.
And address of virtual table is fixed throught inheritance heirarchy.
Again we also that index of function `authenticate()` is fixed throught inheritance heirarchy.
This way right `authenticate()` function execution will be gauranteed. 

Q. What if there is derived class having more that one base class ?
A. We will discuss this scenario in subsequent topic.



### How pure virtual function works
When you declare any function as pure virtual, compiler automatically fills slot of that pure virtual function with dummy function or so called place holder `pure_virtual_called()` library instance. And run time exception is placed if somehow this place holder will be called.  
Rest of calling & virtual table slot mechanism would be same as normal virtual function.
### How pure virtual function works in multiple inheritence.



### Reference 
- http://www.avabodh.com/cxxin/virtualbase.html
- 
