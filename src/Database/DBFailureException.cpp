#include "DBFailureException.h"

DBFailureException::DBFailureException(std::string cause)
	: std::domain_error("DBFailureException: " + cause){}
