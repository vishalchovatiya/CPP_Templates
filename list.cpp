#include <iostream>
#include <algorithm>
#include <list>			
using namespace std;

int main(){
	int arr[] =  {2,3,4,5,6,7};
	
	std::list<int> DLL(arr, arr+6);					// Doubly Linked List
	
	/* Erasing Elements from list */
	DLL.erase(DLL.begin());
	DLL.erase(DLL.begin(), find( DLL.begin(), DLL.end(), 4));
	DLL.remove(2);							// Remove all elements equal to 2
	DLL.remove_if([](int n){ return n > 5; }); 			// Remove all elements greater than 5
	
	/* Manipulating individual Elements from List*/
	DLL.push_front(1);
	cout<<"DLL Front  Element = "<<DLL.front()<<endl;
	DLL.pop_front();	
	DLL.push_back(3);	
	cout<<"DLL Second Element = "<<DLL.back()<<endl;	
	DLL.pop_back();
	
	/* Insert an integer before 1 by searching */
	auto it = find(DLL.begin(), DLL.end(), 1);
	if (it != DLL.end()) {
	    DLL.insert(it, 0);
	}
	
	/* List Capacity Methods */
	if( !DLL.empty() )
		cout<<"DLL Size = "<<DLL.size()<<endl;	
		
	/* Printing List */	
	for( auto i : DLL)
		cout << i << " ";
	cout<<endl;	
	for_each( DLL.begin(), DLL.end(), [] (const int integer) { cout << integer << " ";} );		// -std=c++11 Compiler Option
	
	return 0;
}
