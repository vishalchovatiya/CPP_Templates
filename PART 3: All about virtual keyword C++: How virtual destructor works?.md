### Recap
- We have already learned in our earlier post that how [virtual function]() & [virtual class]() works.
- We left topic virtual destructor in these articles which we will learn now.
- As usual, before learning about virtual destructor we will see why it needed at first place.
### Why we need a virtual destructor?
- We will understand this with our earlier example(slightly twisted) which was as follow:
```
class protocol_t{
	private:
		uint8_t *_type;
		// storage ...
	public:
    protocol_t() { _type = new uint8_t; }
    ~protocol_t() { cout<<"~protocol_t"; delete _type; }

		virtual void authenticate(){};
		virtual void connect(){};
		// operations ...
};

class wifi_t : public protocol_t{
	private:
		char *_pass;
		// storage ...
	public:
    wifi_t() { _pass = new char[15]; }
    ~wifi_t() { cout<<"~wifi_t"; delete _pass; }

		virtual void authenticate(){};
		virtual void connect(){};
		// operations ...
};

class bluetooth_t : public protocol_t{
	private:
		char *_pass;
		// storage ...
	public:
    bluetooth_t() { _pass = new char[15]; }
    ~bluetooth_t(){ cout<<"~wifi_t"; delete _pass; }

		virtual void authenticate(){};
		virtual void connect(){};
		// operations ...
};

void makeConnection(protocol_t *protocol)
{
	protocol->authenticate();
	protocol->connect();
  
  // Do some tx & rx
  
  delete protocol;
}	


int main()
{
	int prot_type = rand()%2;
  
	makeConnection( (prot_type == WIFI) ? static_cast<protocol_t*>(new wifi_t) : static_cast<protocol_t*>(new bluetooth_t));	

	return 0;
}
```
- So, we have created `wifi_t` & `bluetooth_t` objects dynamically in `main()` and passed it to function `makeConnection()`.
- An objects of `wifi_t`, `bluetooth_t` & `protocol_t` also uses heap memory at construction & destruction time.
- Well, this code compiles & runs fine without any error. But when you run above code, at the time of `delete protocol` line it alwasy call destructor of `protocol_t` which you can verify by `~protocol_t` print on console.
- We are freeing only sub-object resources which is `protocol_t` in call of `~protocol_t()` destructor. This means that there is memory leak as we are not freeing heap memory resource of object pointed by pointer `protocol_t` in function `makeConnection()`.
- We even dont know the type of object `protocol_t` pointer pointed to.
- Virtual destructors are there to solve this problem. What we have to do is that
```
virtual ~protocol_t() { cout<<"~protocol_t"; delete _type; }
```
- Put keyword virtual in front of `~protocol_t()` destructor.

### How virtual destructor works?

### Reference 
- http://www.avabodh.com/cxxin/virtualbase.html
- 
