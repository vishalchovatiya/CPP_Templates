#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <queue>
using namespace std;


template<
		typename T,
		typename Compare = std::less<T>
		>
class pq : public std::priority_queue<T, std::vector<T>, Compare>
{
public:
	void print(){
		for(auto it=this->c.begin(); it!= this->c.end(); ++it){
			cout<<*it<<" ";
		}
		cout<<endl;
	}

	bool remove(const T& value) {
		auto it = std::find(this->c.begin(), this->c.end(), value);
		if (it != this->c.end()) {
			this->c.erase(it);
			std::make_heap(this->c.begin(), this->c.end(), this->comp);
			return true;
		}
		return false;
	}

	bool change(const T& oldVal, const T& newVal){
		auto it = std::find(this->c.begin(), this->c.end(), oldVal);
		if (it != this->c.end()) {
			this->c.erase(it);
			this->c.push_back(newVal);
			std::make_heap(this->c.begin(), this->c.end(), this->comp);
			return true;
		}
		return false;
	}
};


int main() {

	pq<pair<int,string>> q;

	q.push(make_pair(0, "0"));
	q.push(make_pair(1, "1"));
	q.push(make_pair(2, "2"));

	q.change(make_pair(1,"1"), make_pair(1,"11"));

	q.remove(make_pair(0,"0"));

	// q.print();

	while(!q.empty()){
		cout<<q.top().first<<" "<<q.top().second<<endl;

		q.pop();
	}


    return 0;
}
