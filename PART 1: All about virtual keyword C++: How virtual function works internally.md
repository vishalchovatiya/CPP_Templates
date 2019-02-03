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
1. Polymorphism(same name many form)
2. Reusability of code(observe `makeConnection` function)
3. Code would be compact(observe earlier `main` function & newer one)

### How virtual function works

### How & where virtual function & table code synthesize ?

### Virtual function support under multiple inheritance

### How virtual destructor behaviour achieved ?

### Reference 
- http://www.avabodh.com/cxxin/virtualbase.html
- 
