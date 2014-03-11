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
		std::vsprintf(f, format, args);
		va_end(args);

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
}

//auto main() -> int {
//}
