### Extracting substring
```
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
### Get list of string separated by delimiter
```
vector<string> getDelimiterSeparatedStrList(string &str, string delimiter, bool  stripMultipleDelimiterSeq = false)
{
  vector<string> resStrList;
  size_t startPos=0;
  size_t endPos=0;
  
  while((endPos = str.find(delimiter, startPos)) != std::string::npos){    
    if(stripMultipleDelimiterSeq && startPos == endPos){
      startPos = endPos+1;
      continue;
    }
    
    resStrList.push_back(str.substr(startPos,endPos-startPos));
    startPos = endPos+1;
  }
  resStrList.push_back(str.substr(startPos));
  return resStrList;
}
```
### Erase substring from main string
```
void eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
	size_t pos = std::string::npos;

	// Search for the substring in string in a loop untill nothing is found
	while ((pos  = mainStr.find(toErase) )!= std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}
```
### Replace substrings from main string
```
string replace(const string &str, const string from, const string to, bool isFirstOnly = false)
{
  string resStr;
  size_t startPos=0;
  size_t endPos=0;
  
  while((endPos = str.find(from, startPos)) != std::string::npos){    
    resStr += str.substr(startPos,endPos-startPos);
    resStr += to;
    startPos = endPos + from.length();
    if(isFirstOnly)break;
  }
  resStr += str.substr(startPos);

  return resStr;
}
```
