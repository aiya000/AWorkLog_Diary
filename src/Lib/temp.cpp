#include <iostream>
#include <string>
#include "StringUtils.hpp"

int main(){
	std::vector<std::string> v = alib::split("command 1 10  120   20 30", ' ');
	for(auto i=v.begin(); i!=v.end(); i++)
		if(*i == "")
			i = v.erase(i)-1;
	std::cout << "size: " << v.size() << std::endl;
	for(auto e : v)
		std::cout << '|' << (e==" "?"s":e) << '|' << std::endl;
	std::cout << std::endl;
}
