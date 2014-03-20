#include "Convert.hpp"

namespace alib {
	time_t stringToTime(const std::string& timestamp){
		std::tm tmStruct;
		strptime(timestamp.c_str(), "%Y-%m-%d %H:%M:%S", &tmStruct);
		tmStruct.tm_isdst = 1;
		time_t t = std::mktime(&tmStruct);
		return t;
	}
	std::string timeToString(time_t time, const char* format){
		char buf[20];
		std::strftime(buf, 20, format, localtime(&time));
		return std::string(buf);
	}
}

//#include <iostream>
//int main(){
//	std::string timeStr = alib::timeToString(time(nullptr));
//	std::cout << timeStr << std::endl;
//	std::cout << alib::stringToTime(timeStr) << std::endl;
//}
