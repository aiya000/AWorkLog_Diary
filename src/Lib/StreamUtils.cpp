#include "StreamUtils.hpp"
#include <iostream>
#include <locale>

namespace alib {
	void unsetStreamDelimiterSpace(std::istream& in){
		struct DelimiterSpace : std::ctype<char> {
			mask rc[table_size];
			DelimiterSpace() : std::ctype<char>([&](){
				rc['\n'] = std::ctype_base::space;
				return &rc[0];
			}()){}
		};
		in.imbue( std::locale(in.getloc(), new DelimiterSpace()) );
	}
}

//#include <sstream>
//int main(){
//	std::stringstream ss;
//	ss << "aaa aaa";
//	alib::unsetStreamDelimiterSpace(ss);
//	std::string str;
//	ss >> str;
//	std::cout << str << std::endl;
//}
