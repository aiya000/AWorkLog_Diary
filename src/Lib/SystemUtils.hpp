#pragma once
#include <stdexcept>
#include <string>

namespace alib {

	/* --==--==--==--==--==--==--==--==--==--==-- */
	class SystemCommandCallException : public std::runtime_error {
	public:
		SystemCommandCallException(std::string cause);
	};

	/* --==--==--==--==--==--==--==--==--==--==-- */

}
