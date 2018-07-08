```
// string::substr
#include <iostream>
#include <string>

#define DEBUG(X)  std::cout<<#X<<" = "<<X<<std::endl;

int main ()
{
  std::string str="We think in generalities, but we live in details.";
                                           // (quoting Alfred N. Whitehead)

  std::string str2 = str.substr (3,5);     // "think"

  std::size_t pos = str.find("live");      // position of "live" in str

  std::string str3 = str.substr (pos);     // get from "live" to the end

  std::string str4 = str.substr (pos, str3.find(" "));     // get from "live" to the end

  DEBUG(str2);
  DEBUG(str3);
  DEBUG(str4);

  return 0;
}
```
