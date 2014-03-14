#pragma once
#include <string>
#include "Action/ActionControl.h"
#include <stdexcept>

class Select {
private:
	/* 認識するコマンドとその列挙 */
	static constexpr int CMD_LEN = 12;
	static const std::string CMD_LIST[CMD_LEN];
	enum CmdEnum {
		LIST   = 0  , LS     = 1  ,
		VIEW   = 2  , 
		EDIT   = 3  , WRITE  = 4  , 
		REEDIT = 5  , REVISE = 6  , 
		REMOVE = 7  , 
		Q      = 8  , EXIT   = 9  , 
		H      = 10 , HELP   = 11
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
	inline void view();
	inline void edit();
	inline void reedit();
	inline void remove();
	inline void help();
	/* --==--==---==--==-- */
};
