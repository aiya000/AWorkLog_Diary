#pragma once
#include <string>
#include "Action/ActionControl.h"
#include <stdexcept>

class Shell {
private:
	/* 認識するコマンドとその列挙 */
	static constexpr int CMD_LEN = 26;
	static const std::string CMD_LIST[CMD_LEN];
	enum CmdEnum {  // <--> CMD_LIST
		LIST      = 0   , LS       = 1   ,
		LISTP     = 2   , LISTPREV = 3   ,
		LISTN     = 4   , LISTNEXT = 5   ,
		FIND      = 6   ,
		SEARCH    = 7   ,
		VIEW      = 8   ,
		EDIT      = 9   , WRITE    = 10  ,
		REEDIT    = 11  , REVISE   = 12  ,
		REMOVE    = 13  , RM       = 14  ,
		BACKUP    = 15  ,
		RESTORE   = 16  ,
		RM_BACKUP = 17  , RMB      = 18  ,
		LS_BACKUP = 19  , LSB      = 20  ,
		Q         = 21  , QUIT     = 22  , EXIT = 23  ,
		H         = 24  , HELP     = 25
	};

	/* 呼び出されるのアクション管理 */
	ActionControl action;

public:
	/* --==- Run -==-- */
	void run();

	/* --==- Launch -==-- */
	// exitならfalse、それ意外ならtrue //
	bool launch(std::string&& cmd) throw(std::invalid_argument);
	bool launch(std::string&  cmd) throw(std::invalid_argument);
	inline std::vector<int> vaToVi(std::vector<std::string>& va);
private:
	/* --==- Util -==-- */
	template <typename T>
	inline T getInput(std::string want="");

	/* --==- Command -==-- */
	inline void list();
	inline void listprev();
	inline void listnext();
	inline void find();       // {{{
	inline void find(std::string& arg);    // }}}
	inline void search();     // {{{
	inline void search(std::string& arg);  // }}}
	inline void view();       // {{{
	inline void view(std::vector<int>&& args);  // }}}
	inline void edit();
	inline void reedit();     // {{{
	inline void reedit(int arg);  // }}}
	inline void remove();     // {{{
	inline void remove(std::vector<int>&& args);  // }}}
	inline void backup();
	inline void restore();    // {{{
	inline void restore(int arg);  // }}}
	inline void rm_backup();  // {{{
	inline void rm_backup(std::vector<int>&& args);  // }}}
	inline void ls_backup();
	inline void help();
	/* --==--==---==--==-- */
};
