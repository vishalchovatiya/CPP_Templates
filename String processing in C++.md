```
// string::substr
#include <iostream>
#include <string>

int main ()
{
  std::string str="We think in generalities, but we live in details.";
                                           // (quoting Alfred N. Whitehead)

  std::string str2 = str.substr (3,5);     // "think"

  std::size_t pos = str.find("live");      // position of "live" in str

  std::string str3 = str.substr (pos);     // get from "live" to the end

  std::string str4 = str.substr (pos, str.find(" "));     // get from "live" to the end

  std::cout << str2 << ' ' << str3 << ' ' << str4 << '\n';

  return 0;
}
```
