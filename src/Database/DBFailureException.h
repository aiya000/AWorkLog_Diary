#pragma once
#include <stdexcept>
#include <string>

class DBFailureException : public std::domain_error {
public:
	DBFailureException(std::string cause);
};
