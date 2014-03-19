#include "SystemUtils.hpp"

namespace alib {

	/* --==--==--==--==--==--==--==--==--==--==-- */
	SystemException::SystemException(std::string cause)
		: std::runtime_error(cause){}
	SystemCommandCallException::SystemCommandCallException(std::string cause)
		: SystemException("SystemCommandCallException" + cause){}
	SystemInterruptedException::SystemInterruptedException(std::string cause)
		: SystemException("SystemInterruptedException" + cause){}

	/* --==--==--==--==--==--==--==--==--==--==-- */

}

