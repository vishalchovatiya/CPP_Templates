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
- But what if we just want single pointer for single Resource & still we can copy it to another smart_pointer? 
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

### 

### References
- https://www.learncpp.com
