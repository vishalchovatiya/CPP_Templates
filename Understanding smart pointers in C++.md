### Intro
- The smart pointers are a really good mechanism to manage dynamically allocated resources & this is the use case where I find C++'s real application as compared to C.
- In this article, we are not going to discuss standard smart pointers provided by library rather we implement our own smart pointer equivalent to it. This will give us an idea of inside working of smart pointers. 
- We will also see how move semantics fits in building smart pointer classes.
- Prior to C++11, standard provided `std::auto_ptr` which had some limitation. But from C++11, standard provided many smart pointers classes. And to understand the need of move semantics we will design our own smart pointer class.
- But before all these nuisances, we will see "why we need smart pointer at 1st place?":

### Why we need smart pointers?
```c++
void func()
{
    Resource *ptr = new Resource;
 
    int x;
    std::cout << "Enter an integer: ";
    std::cin >> x;
 
    if (x == 0)
        throw 0; // the function returns early, and ptr won't be deleted!
                
    if (x < 0)
        return; // the function returns early, and ptr won't be deleted!

    // do stuff with ptr here
 
    delete ptr;
}
```
- In the above code, the early `return` or `throw` statement, causing the function to terminate without variable `ptr` being deleted. Consequently, the memory allocated for variable `ptr` is now leaked (and will be leaked again every time this function is called and returns early).
- These kinds of issues occur because pointer variables have no inherent mechanism to clean up after themselves. 
- We can design our own class of pointer that can do automatic clean-up when sources are no longer used. Here is our class smart pointer:
### `smart_ptr` aka `std::auto_ptr` from C++98
```c++
template<class T>
class smart_ptr
{
    T* m_ptr;
public:
    smart_ptr(T* ptr=nullptr):m_ptr(ptr){}
    
    ~smart_ptr()
    {
        delete m_ptr;
    }
 
    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
};
```
- Now, let's go back to our `func()` example above, and show how a smart pointer class can solve our challenge:
```c++
class Resource
{
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};
 
void func()
{
    smart_ptr<Resource> ptr(new Resource); // ptr now owns the Resource
 
    int x;
    std::cout << "Enter an integer: ";
    std::cin >> x;
 
    if (x == 0)
        throw 0;
                
    if (x < 0)
        return;
 
    // do stuff with ptr here
    
    // dont care about deallocation
}
 
int main()
{
    try{
      func();
    }
    catch(int val){}
 
    return 0;
}
```
- Output
```
Resource acquired
Hi!
Resource destroyed
```
- Note that even in the case where the user enters zero and the function terminates early, the `Resource` is still properly deallocated.
- Because the `ptr` variable is a local variable, `ptr` will be destroyed when the function terminates (regardless of how it terminates). And because the `smart_ptr` destructor will clean up the `Resource`, we are assured that the `Resource` will be properly cleaned up.
- There is still some problem with our code. Like:
```c++
int main()
{
    smart_ptr<Resource> res1(new Resource);
    smart_ptr<Resource> res2(res1); // Alternatively, don't initialize res2 and then assign res2 = res1;
 
    return 0;
}
```
- Output
```
Resource acquired
Resource destroyed
Resource destroyed
```
- In this case destructor of our `Resource` object will be called twice which can crash the program. 
- What if, instead of having our copy constructor and assignment operator copy the pointer ("copy semantics"), we instead transfer/move ownership of the pointer from the source to the destination object? This is the core idea behind move semantics. Move semantics means the class will transfer ownership of the object rather than making a copy.
- Let's update our `smart_ptr` class to show how this can be done:
```c++
template<class T>
class smart_ptr
{
    T* m_ptr;
public:
    smart_ptr(T* ptr=nullptr) :m_ptr(ptr) {}
    
    ~smart_ptr()
    {
        delete m_ptr;
    }
 
    // copy constructor that implements move semantics
    smart_ptr(smart_ptr& a) // note: not const
    {
        m_ptr = a.m_ptr; // transfer our dumb pointer from the source to our local object
        a.m_ptr = nullptr; // make sure the source no longer owns the pointer
    }
    
    // assignment operator that implements move semantics
    smart_ptr& operator=(smart_ptr& a) // note: not const
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
};
 
class Resource
{
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};
 
int main()
{
    smart_ptr<Resource> res1(new Resource);
    smart_ptr<Resource> res2(res1);
 
    return 0;
}
```
- Output
```
Resource acquired
Resource destroyed
```
### `std::auto_ptr`, and why to avoid it
- What we have seen above as `smart_ptr` is basically an `std::auto_ptr` which was introduced in C++98, was C++'s first attempt at a standardized smart pointer.
- However, `std::auto_ptr` (and our `smart_ptr` class) has a number of problems that make using it dangerous.
  1. Because `std::auto_ptr` implements move semantics through the copy constructor and assignment operator, passing an `std::auto_ptr` by value to a function will cause your resource to get moved to the function parameter (and be destroyed at the end of the function when the function parameters go out of scope). Then when you go to access your `std::auto_ptr` argument from the caller (not realizing it was transferred and deleted), you're suddenly dereferencing a null pointer. Crash!
  2. `std::auto_ptr` always deletes its contents using non-array delete. This means `std::auto_ptr` won't work correctly with dynamically allocated arrays, because it uses the wrong kind of deallocation. Worse, it won't prevent you from passing it a dynamic array, which it will then mismanage, leading to memory leaks.
- Because of the above-mentioned shortcomings, `std::auto_ptr` has been deprecated in C++11, and it should not be used. In fact, `std::auto_ptr` is slated for complete removal from the standard library as part of C++17!

- Overriding the copy semantics to implement move semantics leads to weird edge cases and inadvertent bugs. Because of this, in C++11, the concept of "move" was formally defined, and "move semantics" were added to the language to properly differentiate copying from moving. In C++11, `std::auto_ptr` has been replaced by a bunch of other types of "move-aware" smart pointers: `std::scoped_ptr`, `std::unique_ptr`, `std::weak_ptr`, and `std::shared_ptr`. 
- We'll also explore the two most popular of these: `std::unique_ptr` (which is a direct replacement for `std::auto_ptr`) and `std::shared_ptr`.

### `std::unique_ptr`
- `std::unique_ptr` is the C++11 replacement for `std::auto_ptr`. It should be used to manage any dynamically allocated object that is not shared by multiple objects. That is, `std::unique_ptr` should completely own the object it manages, not share that ownership with other classes.
- We can convert our `smart_ptr` we designed above into `std::unique_ptr`. And for that one thing, we can do is delete the copy constructor & assignment operator so that no one can copy smart pointer. 
- As we are not allowing a copy of smart pointer we can't pass our smart pointer to any function by value or return by value. And this is not good design. 
- To pass or return by value, we can add move constructor & move assignment operator, so that while passing or returning by value, we would have to transfer ownership through move semantics. This way we can also ensure single ownership throughout the lifetime of the object.
```c++
template<class T>
class smart_ptr
{
    T* m_ptr;
public:
    smart_ptr(T* ptr = nullptr) : m_ptr(ptr){}
 
    ~smart_ptr()
    {
        delete m_ptr;
    }
 
    // Copy constructor
    smart_ptr(const smart_ptr& a) = delete;
 
    // Move constructor
    smart_ptr(smart_ptr&& a) : m_ptr(a.m_ptr)
    {
        a.m_ptr = nullptr;
    }
 
    // Copy assignment
    smart_ptr& operator=(const smart_ptr& a) = delete;
 
    // Move assignment
    smart_ptr& operator=(smart_ptr&& a)
    {        
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
 
smart_ptr<Resource> func(smart_ptr<Resource> temp) 
{
  // Do something
  return temp;
}

int main()
{
    smart_ptr<Resource> res1(new Resource);
    // smart_ptr<Resource> res3 = res1; // Won't compile, as copy contructor is deleted
    smart_ptr<Resource> res3 = func(std::move(res1)); // calls move semantics
 
    return 0;
}
```
- Output
```
Resource acquired
Resource destroyed
```
- This is not the exact implementation of `std::unique_ptr` as there is deleter, implicit cast to bool & other security features are included in an actual implementation, but this gives you a bigger picture of how `std::unique_ptr` is implemented.

### `std::shared_ptr`
- In some cases, we have a requirement where a single resource is represented by multiple pointers. We can not accomplish this by `std::unique_ptr`. To accomplish this, we can add a new variable to our smart pointer class which keeps track of reference count at the real time, and when the reference count goes to zero which means nobody is using that resource, we will deallocate that resource.
- Unlike `std::unique_ptr`, which is designed to singly own and manage a resource, `std::shared_ptr` is meant to solve the case where you need multiple smart pointers co-owning a resource.
```c++
template<class T>
class smart_ptr
{
    T* m_ptr;
    uint32_t *m_refCount;
public:
    smart_ptr(T* ptr = nullptr):m_ptr(ptr)
    {
        if(m_ptr)
            m_refCount = new uint32_t(1);
        else
            m_refCount = nullptr;    
    }
 
    ~smart_ptr()
    {
        if(m_refCount != nullptr){
            (*m_refCount)--;
            if((*m_refCount) == 0){
                delete m_ptr;
                delete m_refCount;
            }
        } 
    }
 
    // Copy constructor
    smart_ptr(const smart_ptr& a)
    {
        m_ptr = a.m_ptr;
        m_refCount = a.m_refCount;
        (*m_refCount)++;
    }
 
    // Move constructor
    smart_ptr(smart_ptr&& a): m_ptr(a.m_ptr), m_refCount(a.m_refCount)
    {
        a.m_ptr = nullptr;
        a.m_refCount = nullptr;
    }
 
    // Copy assignment
    smart_ptr& operator=(const smart_ptr& a)
    {
        m_ptr = a.m_ptr;
        m_refCount = a.m_refCount;
        (*m_refCount)++;
        return *this;
    }
 
    // Move assignment
    smart_ptr& operator=(smart_ptr&& a)
    {
        if (&a == this)
            return *this;
 
        delete m_ptr;
        delete m_refCount;
 
        m_ptr = a.m_ptr;
        a.m_ptr = nullptr;
 
        m_refCount = a.m_refCount;
        a.m_refCount = nullptr;

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
 
smart_ptr<Resource> func(smart_ptr<Resource> temp) 
{
  // Do something
  return temp;
}

int main()
{
    Resource *res = new Resource;
    smart_ptr<Resource> ptr1(res);
    {
        smart_ptr<Resource> ptr2(ptr1); 
            auto ptr3 = func(ptr1);     
        std::cout << "Killing one shared pointer\n";    
    }
    std::cout << "Killing another shared pointer\n";
 
    return 0;
}
```
- Unlike `std::unique_ptr`, which uses a single pointer internally, `std::shared_ptr` uses two pointers internally. One pointer points at the resource being managed. The other points at a **"control block", which is a dynamically allocated object** that tracks of a bunch of stuff, including how many `std::shared_ptr` are pointing at the resource. 
- Here I have only used single variable to keep track of references pointing to resource for simplicity. The actual implementation is a bit bulky for more feature & security purpose.

### Bit about move constructor & move assignment operator
##### When are the move constructor and move assignment called?
- The move constructor and move assignment are called when those functions have been defined, and the argument for construction or assignment is an `r-value`. Most typically, this `r-value` will be a literal or temporary value.
- In most cases, a move constructor and move assignment operator will not be provided by default, unless the class does not have any defined copy constructors, copy assignment, move assignment, or destructors. However, the default move constructor and move assignment do the same thing as the default copy constructor and copy assignment (**make copies, not do moves**).

##### `l-value` reference & `r-value` reference
- I have already written separate [article](https://github.com/VisheshPatel/CPP_Templates/blob/master/Understanding%20l-value%2C%20r-value%2C%20l-value%20reference%2C%20r-value%20reference.md) for that. 

##### `std::move`
- In C++11, `std::move` is a standard library function that serves a single purpose -- **to convert its argument into an `r-value`**.
- Once you start using move semantics more regularly, you'll start to find cases where you want to invoke move semantics, but the objects you have to work with are `l-values`, not `r-values`. Consider the following `swap()` function as an example:
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
- Above `swap()` function makes 3 copies. That leads to a lot of excessive string creation and destruction, which is slow.
- However, doing copies isn't necessary here. All we're really trying to do is swap the values of `a` and `b`, which can be accomplished just as well using 3 moves instead! So if we switch from copy semantics to move semantics, we can make our code more performant.
```c++
template<class T>
void swap(T& a, T& b) 
{ 
  T tmp { `std::move`(a) }; // invokes move constructor
  a = std::move(b); // invokes move assignment
  b = std::move(tmp); // invokes move assignment
}
```
- `std::move` can also be useful when sorting an array of elements. Many sorting algorithms (such as selection sort and bubble sort) work by swapping pairs of elements. Here we can use move semantics, which is more efficient.
### References
- https://www.geeksforgeeks.org/smart-pointers-cpp/
- https://www.learncpp.com/cpp-tutorial/15-1-intro-to-smart-pointers-move-semantics/
- https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
- https://docs.microsoft.com/en-us/cpp/cpp/smart-pointers-modern-cpp?view=vs-2017
