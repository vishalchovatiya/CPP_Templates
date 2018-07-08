### Printing `vector` in forward & reverse order
```
vector<int>     vArr(arr, arr+N);
std::for_each(vArr.begin(), vArr.end(), [](int a){cout<<a<<" ";}); // Forward
std::for_each(vArr.rbegin(), vArr.rend(), [](int a){cout<<a<<" ";}); // Reverse
```
