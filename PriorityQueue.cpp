#include<iostream>
#include <queue>

using namespace std;

/*
	Question:- Min & Max heap using C++ STL
	
	template< class T, 
			  class Container = std::vector<T>, 
			  class Compare = std::less<typename Container::value_type>
			
	>    class priority_queue;
	
	Solution:- Use Priority Queue
*/

int main()
{
	// Declaring Queue	
	struct
	{
		bool operator()(pair<int,string> const &a, pair<int,string> const &b)
		{
			return a.first > b.first;	// This statement make Min & Max heap
		}
	}compare;
	
	priority_queue< pair<int,string>, vector<pair<int,string>>, decltype(compare)>  q;		// decltype extract the data type similar as sizeof in c
	
	// Initializing Queue	
	q.push( make_pair( 1, "RAM" ));
	q.push( make_pair( 5, "KAM" ));
	q.push( make_pair( 3, "JAM" ));
	q.push( make_pair( 2, "BAM" ));
	q.push( make_pair( 4, "NAM" ));	
	
	// Printing
	while( !q.empty())
	{
		cout<<q.top().second<<" ";
		q.pop();
	}
 
    return 0;
}
