#include "SystemUtils.hpp"

namespace alib {

	/* --==--==--==--==--==--==--==--==--==--==-- */
	SystemCommandCallException::SystemCommandCallException(std::string cause)
		: std::runtime_error("SystemCommandCallException" + cause){}

	/* --==--==--==--==--==--==--==--==--==--==-- */

}
