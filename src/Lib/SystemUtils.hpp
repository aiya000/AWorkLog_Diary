#pragma once
#include <stdexcept>
#include <string>

namespace alib {

	/* --==--==--==--==--==--==--==--==--==--==-- */
	struct SystemException : public std::runtime_error {
		SystemException(std::string cause);
	};
	struct SystemCommandCallException : SystemException {
		SystemCommandCallException(std::string cause);
	};
	// 中断したい時、割り込みたいとき。
	struct SystemInterruptedException : SystemException {
		SystemInterruptedException(std::string cause);
	};

	/* --==--==--==--==--==--==--==--==--==--==-- */

}
