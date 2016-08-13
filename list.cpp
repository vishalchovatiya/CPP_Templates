#include <iostream>
#include <algorithm>
#include <list>
using namespace std;

void printList(list<int>	l){
	for_each( l.begin(), l.end(), [] (const int integer) { cout << integer << " ";} );		// -std=c++11 Compiler Option
	cout<<endl;
}

int main(){
	int arr[] =  {2,3,4,5,6,7};
	
	std::list<int> DLL(arr, arr+6);					// Doubly Linked List
	
	DLL.erase(DLL.begin());
	DLL.erase(DLL.begin(), DLL.end());
	
	DLL.push_front(1);
    DLL.push_back(3);
    
    cout<<DLL.front()<<endl;
    cout<<DLL.back()<<endl;
    
    DLL.pop_front();
    DLL.pop_back();
	
	DLL.remove(2);								// Remove all elements equal to 2
	DLL.remove_if([](int n){ return n > 5; }); 	// Remove all elements greater than 5

	// Insert an integer before 1 by searching
    auto it = find(DLL.begin(), DLL.end(), 1);
    if (it != DLL.end()) {
        DLL.insert(it, 0);
    }
	
	for( auto i : DLL)
		cout << i << " ";
		
	if( !DLL.empty() )
		cout<<" DLL Size = "<<DLL.size()<<endl;	
	
	return 0;
}
