#include <iostream>
#include <vector>
#include <string>
#include <functional> 	// std::minus
#include <numeric>		// std::accumulate

using namespace std;

int myfunction (int lastfuncCallResult, int y) {
	return 2*y;
}

string convertInString(string result, int x){
	return result.empty() ? itoa(x) : result + "-" + itoa(x); 
}

class myclass {
	public:
	int operator()(int lastfuncCallResult, int y) {
		return 2*y;
	}
} myobject;

int main () {
	int init = 100;
	int numbers[] = {10,20,30};
	vector<int>		V(numbers, numbers+4);

	cout<<accumulate( V.begin(), V.end(), init)<<endl;				// 100 + 10 + 20 +30

	cout<<accumulate( V.begin(), V.end(), init, minus<int>())<<endl;		// 100 - 10 - 20 - 30
	
	cout<<accumulate ( V.begin (), V.end (), 1, multiplies<int>())<<endl;		//10*20*30
	
	cout<<accumulate( V.begin(), V.end(), init, myfunction)<<endl;

	cout<<accumulate( V.begin(), V.end(), init, myobject)<<endl;
                             
	return 0;
}
