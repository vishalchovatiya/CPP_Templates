### Printing `vector` in forward & reverse order
```
vector<int>     vArr(arr, arr+N);
std::for_each(vArr.begin(), vArr.end(), [](int a){cout<<a<<" ";}); // Forward
std::for_each(vArr.rbegin(), vArr.rend(), [](int a){cout<<a<<" ";}); // Reverse
```
### Printing `map` in forward & reverse order
```
for_each(map.begin(),map.end(),[](pair<const string,Node> &pair) 
{
  cout<<pair.first<<endl;
  cout<<pair.second<<endl;
});
```
Note: first template parameter of pair in lambda function should be **const**. 
