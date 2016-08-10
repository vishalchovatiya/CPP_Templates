#include <iostream>     
#include <algorithm>    // std::sort
#include <vector>       

bool compareFunction(int i,int j) { 
  return (i<j); 
}

struct compareObject {
  bool operator() (int i,int j) { 
    return (i<j);
  }
} myobject;

int main () {
  int myints[] = {32,71,12,45,26,80,53,33};
  
  std::vector<int> myvector (myints, myints+8);               // 32 71 12 45 26 80 53 33

  // using default comparison (operator <):
  std::sort (myvector.begin(), myvector.end());           //(12 32 45 71)26 80 53 33
  
  std::sort (myvector.begin()+4, myvector.end(), compareFunction); // 12 32 45 71(26 33 53 80)

  // using object as comp
  std::sort (myvector.begin(), myvector.end(), compareObject);     //(12 26 32 33 45 53 71 80)

  for (std::vector<int>::iterator i=myvector.begin(); i!=myvector.end(); i++)
    std::cout << ' ' << *i;

  return 0;
}
