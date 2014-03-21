#pragma once
#include <string>
#include "Action/ActionControl.h"
#include <stdexcept>

class Select {
private:
	/* 認識するコマンドとその列挙 */
	static constexpr int CMD_LEN = 16;
	static const std::string CMD_LIST[CMD_LEN];
	enum CmdEnum {  // <--> CMD_LIST
		LIST   = 0   , LS       = 1   ,
		LISTP  = 2   , LISTPREV = 3   ,
		LISTN  = 4   , LISTNEXT = 5   ,
		VIEW   = 6   , 
		EDIT   = 7   , WRITE    = 8   , 
		REEDIT = 9   , REVISE   = 10  , 
		REMOVE = 11  , 
		Q      = 12  , EXIT     = 13  , 
		H      = 14  , HELP     = 15
	};

	/* 呼び出されるのアクション管理 */
	ActionControl action;

public:
	/* --==- Run -==-- */
	void run();

	/* --==- Launch -==-- */
	// exitならfalse、それ意外ならtrue //
	bool launch(std::string&& cmd) throw(std::invalid_argument);
private:
	/* --==- Util -==-- */
	inline int getInput();

	/* --==- Command -==-- */
	inline void list();
	inline void listprev();
	inline void listnext();
	inline void view();
	inline void edit();
	inline void reedit();
	inline void remove();
	inline void help();
	/* --==--==---==--==-- */
};
