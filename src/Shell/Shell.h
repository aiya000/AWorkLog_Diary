#pragma once
#include <string>
#include "Action/ActionControl.h"
#include <stdexcept>

class Shell {
private:
	/* 認識するコマンドとその列挙 */
	static constexpr int CMD_LEN = 18;
	static const std::string CMD_LIST[CMD_LEN];
	enum CmdEnum {  // <--> CMD_LIST
		LIST   = 0   , LS       = 1   ,
		LISTP  = 2   , LISTPREV = 3   ,
		LISTN  = 4   , LISTNEXT = 5   ,
		FIND   = 6   ,
		SEARCH = 7   ,
		VIEW   = 8   , 
		EDIT   = 9   , WRITE    = 10  , 
		REEDIT = 11  , REVISE   = 12  , 
		REMOVE = 13  , 
		Q      = 14  , EXIT     = 15  , 
		H      = 16  , HELP     = 17
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
	template <typename T>
	inline T getInput(std::string want="");

	/* --==- Command -==-- */
	inline void list();
	inline void listprev();
	inline void listnext();
	inline void find();
	inline void search();
	inline void view();
	inline void edit();
	inline void reedit();
	inline void remove();
	inline void help();
	/* --==--==---==--==-- */
};
