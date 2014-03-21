#pragma once
#include <string>
#include <ctime>

namespace alib {
	time_t      stringToTime(const std::string& timestamp);  // まだ指定書式でないと使えない。
	std::string timeToString(time_t time, const char* format="%Y-%m-%d %H:%M");
}
