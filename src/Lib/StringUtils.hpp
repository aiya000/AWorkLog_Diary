#include <string>
#include "SystemUtils.hpp"

namespace alib {
	/**
	 * Usage:
	 *   std::string str = alib::format("%d-%f-%s", 10, 20.0, "30");
	 */
	std::string format(const char* format, ...);
	std::string format(const std::string& format, ...);

	// refer to http://program.station.ez-net.jp/special/handbook/cpp/string/trim.asp
	std::string trim(const std::string&  str, const char* trimChars=" \t\v\r\n");
	std::string trim(const std::string&& str, const char* trimChars=" \t\v\r\n");
}
