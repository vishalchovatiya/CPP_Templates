### Intro
- It is obvious that there is relation between smart pointers & move semantics just by looking at tittle of this article
- C++ provide smart pointers as standard feature but to understand need of move semantics we will design our own smart pointer class.

### Why we need smart pointers?
```c++
void someFunction()
{
    Resource *ptr = new Resource;
 
    int x;
    std::cout << "Enter an integer: ";
    std::cin >> x;
 
    if (x == 0)
        throw 0; // the function returns early, and ptr won’t be deleted!
                
    if (x < 0)
        return; // the function returns early, and ptr won’t be deleted!
 
    // do stuff with ptr here
 
    delete ptr;
}
```
- In the above two programs, the early return or throw statement execute, causing the function to terminate without variable ptr being deleted. Consequently, the memory allocated for variable ptr is now leaked (and will be leaked again every time this function is called and returns early).
- At heart, these kinds of issues occur because pointer variables have no inherent mechanism to clean up after themselves.
- We can design our own class of pointer that can do automatic clean-up when sources is no longer used.

- Here is our class smart pointer
```c++
template<class T>
class smart_pointer
{
	T* m_ptr;
public:
	smart_pointer(T* ptr=nullptr):m_ptr(ptr){}
	
	~smart_pointer()
	{
		delete m_ptr;
	}
 
	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }
};
```
- Now let’s go back to our someFunction() example above, and show how a smart pointer class can solve our challenge:
```c++
class Resource
{
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};
 
void someFunction()
{
    smart_pointer<Resource> ptr(new Resource); // ptr now owns the Resource
 
    int x;
    std::cout << "Enter an integer: ";
    std::cin >> x;
 
    if (x == 0)
        return; // the function returns early
 
    // do stuff with ptr here
    ptr->sayHi();
}
 
int main()
{
    someFunction();
 
    return 0;
}
```
- Output
```
Resource acquired
Hi!
Resource destroyed
```
- Note that even in the case where the user enters zero and the function terminates early, the Resource is still properly deallocated.
- Because the ptr variable is a local variable, ptr will be destroyed when the function terminates (regardless of how it terminates). And because the Auto_ptr1 destructor will clean up the Resource, we are assured that the Resource will be properly cleaned up.
### Flaws
- There is still some problem with our code. Like:
```c++
int main()
{
	smart_pointer<Resource> res1(new Resource);
	smart_pointer<Resource> res2(res1); // Alternatively, don't initialize res2 and then assign res2 = res1;
 
	return 0;
}
```
- Output
```
Resource acquired
Resource destroyed
Resource destroyed
```
- In this case destructor of our Resource object will be called twice which can crash the program. One thing we can do is delete the copy constructor so that no one can copy smart_pointer
```
Auto_ptr1(const Auto_ptr1 &rhs)	 = delete;
```
- As we are not allowing copy of smart pointer we can not pass our smart_pointer to any function by value or return by value. And this is not good design. Instead of deleting copy constructor we can count reference of copy, when this reference goes to zero we will delete/free our Resource. 
- This is good idea but this will create many smart_pointer pointing to same resource & can cause confusion & messy code. Althoug C++ standard already provide this kind of smart_pointer named as shared_pointer. 
- But what if we just want single pointer for single Resource(yes! like std::unique_ptr) & still we can copy it to another smart_pointer? 
- What if, instead of having our copy constructor and assignment operator copy the pointer (“copy semantics”), we instead transfer/move ownership of the pointer from the source to the destination object? This is the core idea behind move semantics. Move semantics means the class will transfer ownership of the object rather than making a copy.
- Let's update our Auto_ptr1 class to show how this can be done:
```c++
template<class T>
class smart_pointer
{
	T* m_ptr;
public:
	smart_pointer(T* ptr=nullptr)
		:m_ptr(ptr)
	{
	}
	
	~smart_pointer()
	{
		delete m_ptr;
	}
 
	// copy constructor that implements move semantics
	smart_pointer(smart_pointer& a) // note: not const
	{
		m_ptr = a.m_ptr; // transfer our dumb pointer from the source to our local object
		a.m_ptr = nullptr; // make sure the source no longer owns the pointer
	}
	
	// assignment operator that implements move semantics
	smart_pointer& operator=(smart_pointer& a) // note: not const
	{
		if (&a == this)
			return *this;
 
		delete m_ptr; // make sure we deallocate any pointer the destination is already holding first
		m_ptr = a.m_ptr; // then transfer our dumb pointer from the source to the local object
		a.m_ptr = nullptr; // make sure the source no longer owns the pointer
		return *this;
	}
 
	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }
	bool isNull() const { return m_ptr == nullptr;  }
};
 
class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};
 
int main()
{
	smart_pointer<Resource> res1(new Resource);
	smart_pointer<Resource> res2(res1);
 
	return 0;
}
```
- Output
```
Resource acquired
Resource destroyed
```
### `std::auto_ptr`, and why to avoid it
- What we have seen above as smart_pointer is  basically a `std::auto_ptr` which was introduced in C++98, was C++’s first attempt at a standardized smart pointer.
- However, std::auto_ptr (and our smart_pointer class) has a number of problems that makes using it dangerous.
  1. Because std::auto_ptr implements move semantics through the copy constructor and assignment operator, passing a std::auto_ptr by value to a function will cause your resource to get moved to the function parameter (and be destroyed at the end of the function when the function parameters go out of scope). Then when you go to access your auto_ptr argument from the caller (not realizing it was transferred and deleted), you’re suddenly dereferencing a null pointer. Crash!
  2. std::auto_ptr always deletes its contents using non-array delete. This means auto_ptr won’t work correctly with dynamically allocated arrays, because it uses the wrong kind of deallocation. Worse, it won’t prevent you from passing it a dynamic array, which it will then mismanage, leading to memory leaks.
- Because of the above mentioned shortcomings, std::auto_ptr has been deprecated in C++11, and it should not be used. In fact, std::auto_ptr is slated for complete removal from the standard library as part of C++17!

- Overriding the copy semantics to implement move semantics leads to weird edge cases and inadvertent bugs. Because of this, in C++11, the concept of “move” was formally defined, and “move semantics” were added to the language to properly differentiate copying from moving. In C++11, std::auto_ptr has been replaced by a bunch of other types of “move-aware” smart pointers: std::scoped_ptr, std::unique_ptr, std::weak_ptr, and std::shared_ptr. We’ll also explore the two most popular of these: unique_ptr (which is a direct replacement for auto_ptr) and shared_ptr.

### std::unique_ptr
```c++
template<class T>
class Auto_ptr4
{
	T* m_ptr;
public:
	Auto_ptr4(T* ptr = nullptr):m_ptr(ptr){}
 
	~Auto_ptr4()
	{
		delete m_ptr;
	}
 
	// Copy constructor
	Auto_ptr4(const Auto_ptr4& a) = delete;
 
	// Move constructor
	Auto_ptr4(Auto_ptr4&& a)
		: m_ptr(a.m_ptr)
	{
    cout<<"MOVED\n";
		a.m_ptr = nullptr;
	}
 
	// Copy assignment
	Auto_ptr4& operator=(const Auto_ptr4& a) = delete;
 
	// Move assignment
	Auto_ptr4& operator=(Auto_ptr4&& a)
	{
    cout<<"MOVED\n";
		
		if (&a == this)
			return *this;
 
		delete m_ptr;
 
		m_ptr = a.m_ptr;
		a.m_ptr = nullptr;
 
		return *this;
	}
 
	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }
};
 
class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};
 
Auto_ptr4<Resource> func(Auto_ptr4<Resource> temp) 
{
  // Do something
  return temp;
}

int main()
{
	Auto_ptr4<Resource> res1(new Resource);
	Auto_ptr4<Resource> res3 = func(std::move(res1));
 
	return 0;
}
```
- Output
```
Resource acquired
MOVED
MOVED
Resource destroyed
```
- As you can see we are moving our pointer as copy constructor & assignment operator methods are deleted. This is not the exact implementation of std::unique_ptr as there is deleter & other security features are there, but gives you bigger picture of how unique_ptr is implemented.
### std::shared_ptr
```

```
### Bit about move constructor & move assignment operator
##### When are the move constructor and move assignment called?
- The move constructor and move assignment are called when those functions have been defined, and the argument for construction or assignment is an r-value. Most typically, this r-value will be a literal or temporary value.
- In most cases, a move constructor and move assignment operator will not be provided by default, unless the class does not have any defined copy constructors, copy assignment, move assignment, or destructors. However, the default move constructor and move assignment do the same thing as the default copy constructor and copy assignment (**make copies, not do moves**).

##### l-value reference & r-value reference
- I have already written separate article for that. 

##### std::move
- In C++11, std::move is a standard library function that serves a single purpose -- **to convert its argument into an r-value**.
- Once you start using move semantics more regularly, you’ll start to find cases where you want to invoke move semantics, but the objects you have to work with are l-values, not r-values. Consider the following swap function as an example:
```c++
template<class T>
void swap(T& a, T& b) 
{ 
  T tmp { a }; // invokes copy constructor
  a = b; // invokes copy assignment
  b = tmp; // invokes copy assignment
}
 
int main()
{
	std::string x{ "abc" };
	std::string y{ "de" };

	swap(x, y);

	return 0;
}
```
- Above function swap makes 3 copies. That leads to a lot of excessive string creation and destruction, which is slow.
- However, doing copies isn’t necessary here. All we’re really trying to do is swap the values of a and b, which can be accomplished just as well using 3 moves instead! So if we switch from copy semantics to move semantics, we can make our code more performant.
```c++
template<class T>
void swap(T& a, T& b) 
{ 
  T tmp { std::move(a) }; // invokes move constructor
  a = std::move(b); // invokes move assignment
  b = std::move(tmp); // invokes move assignment
}
```
- std::move can also be useful when sorting an array of elements. Many sorting algorithms (such as selection sort and bubble sort) work by swapping pairs of elements. Here we can use move semantics, which is more efficient.
- Bottom line: std::move can be used whenever we want to treat an l-value like an r-value.
### References
- https://www.learncpp.com
