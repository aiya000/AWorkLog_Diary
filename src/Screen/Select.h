#pragma once
#include <string>
#include "Action/ActionControl.h"
#include <stdexcept>

class Select {
private:
	static constexpr int CMD_LEN = 10;
	static const std::string CMD_LIST[CMD_LEN];
	enum CmdEnum {
		VIEW   = 0 , 
		EDIT   = 1 , WRITE  = 2 , 
		REEDIT = 3 , REVISE = 4 , 
		REMOVE = 5 , 
		Q      = 6 , EXIT   = 7 , 
		H      = 8 , HELP   = 9
	};
	ActionControl action;

public:
	inline void usage();

	// exitならfalse、それ意外ならtrue //
	bool launch(std::string&& cmd) throw(std::invalid_argument);
	void run();
};
