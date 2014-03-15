#pragma once
#include <iostream>

namespace alib {
	// refer to http://stackoverflow.com/questions/7302996/changing-the-delimiter-for-cin-c
	void unsetStreamDelimiterSpace(std::istream& in);
}
