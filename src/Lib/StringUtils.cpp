#include "StringUtils.hpp"
#include <iostream>
#include <cstdio>
#include <cstdarg>

namespace alib {

	/* --==--==--==--==--==--==--==--==--==--==-- */
	std::string format(const char* format, ...){
		char f[512];
		va_list args;

		va_start(args, format);
		std::vsprintf(f, format, args); va_end(args);

		return std::string(f);
	}
	std::string format(const std::string& format, ...){
		char f[512];
		va_list args;

		va_start(args, format);
		std::vsprintf(f, format.c_str(), args);
		va_end(args);

		return std::string(f);
	}

	/* --==--==--==--==--==--==--==--==--==--==-- */
	std::string trim(const std::string& str, const char* trimChars){
		std::string::size_type left = str.find_first_not_of(trimChars);
		if(left != std::string::npos){
			std::string::size_type right = str.find_last_not_of(trimChars);
			return str.substr(left, right - left + 1);
		}
		return str;
	}
	std::string trim(const std::string&& str, const char* trimChars){
		return trim(str, trimChars);
	}

	/* --==--==--==--==--==--==--==--==--==--==-- */
	std::vector<std::string> split(const std::string& str, const char delimiter){
		std::vector<std::string> devided;
		std::string sp = alib::trim(str);
		int current = 0, i;
		for(i=0; i<sp.size(); i++){
			if(sp[i] == delimiter){
				devided.push_back( sp.substr(current, i-current) );
				current = i+1;
			}
		}
		devided.push_back( sp.substr(current) );
		return devided;
	}
	std::vector<std::string> split(const std::string&& str, const char delimiter){
		return split(str, delimiter);
	}

	/* --==--==--==--==--==--==--==--==--==--==-- */
}

//auto main() -> int {
//}
