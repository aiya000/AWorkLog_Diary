#pragma once
#include <string>
#include "Action/ActionControl.h"
#include <stdexcept>

class Select {
private:
	static constexpr int CMD_LEN = 7;
	static const std::string CMD_LIST[CMD_LEN];
	//const std::string Select::CMD_LIST[] = {
	//	"view",
	//	"edit",
	//	"remove",
	//	"q",
	//	"exit",
	//	"h",
	//	"help"
	//};
	ActionControl action;

public:
	// exitならfalse、それ意外ならtrue //
	bool launch(std::string&& cmd) throw(std::invalid_argument);
	void run();
};
