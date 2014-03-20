#pragma once
#include <string>
#include <ctime>

namespace alib {
	time_t      stringToTime(const std::string& timestamp);
	std::string timeToString(time_t time, const char* format="%Y-%m-%d");
}
