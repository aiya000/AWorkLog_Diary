#include "Shell.h"
#include <iostream>
#include <string>
#include "../Lib/StringUtils.hpp"
#include <cassert>

// This synchronize order with enum CmdEnum.
const std::string Shell::CMD_LIST[] = {
	"list"      , "ls"       , 
	"listp"     , "listprev" , 
	"listn"     , "listnext" , 
	"find"      ,
	"search"    ,
	"view"      , 
	"edit"      , "write"    , 
	"reedit"    , "revise"   , 
	"remove"    , 
	"backup"    ,
	"restore"   ,
	"ls_backup" ,
	"q"         , "exit"     , 
	"h"         , "help"
};

/* --==--==--==--== Run ==--===-==--==-- */
void Shell::run(){
	std::cout << "[help] to viewing help" << std::endl << std::endl;
	while(true){
		std::cout << "awork$ ";
		
		std::string cmd;  std::cin >> cmd;
		try{
			bool noexit = this->launch( alib::trim(cmd) );
			if(!noexit)  break;
		}catch(std::invalid_argument e){
			std::cerr << e.what() << ": " << cmd << std::endl;
		}
	}
}


/* --==--==--==--== Launch ==--===-==--==-- */
bool Shell::launch(std::string&& cmd) throw(std::invalid_argument){

	/* 渡された引数が何番目のコマンドか判定 */
	int i;
	for(i=0; i<CMD_LEN; i++)
		if(cmd == CMD_LIST[i])
			break;
	if(i == CMD_LEN)
		throw std::invalid_argument("command not found");

	/* 列挙子として判定 */
	switch( static_cast<CmdEnum>(i) ){
		case LIST:
		case LS:
			this->list();
			break;
		case LISTP:
		case LISTPREV:
			this->listprev();
			break;
		case LISTN:
		case LISTNEXT:
			this->listnext();
			break;
		case FIND:
			this->find();
			break;
		case SEARCH:
			this->search();
			break;
		case VIEW:
			this->view();
			break;
		case EDIT:
		case WRITE:
			this->edit();
			break;
		case REEDIT:
		case REVISE:
			this->reedit();
			break;
		case REMOVE:
			this->remove();
			break;
		case BACKUP:
			this->backup();
			break;
		case RESTORE:
			this->restore();
			break;
		case LS_BACKUP:
			this->ls_backup();
			break;
		case Q:
		case EXIT:
			return false;
		case H:
		case HELP:
			this->help();
			break;
		default:
			assert(!"Shell#launch -> invalid case");
	}

	return true;
}


/* --==--==--==--== Util ==--===-==--==-- */
template <typename T>
inline T Shell::getInput(std::string want){
	std::cout << "Please Input " + want + " (abort: 0)" << std::endl << ">> ";
	T input;
	std::cin >> input;
	if(!std::cin.good()){
		std::cin.clear();
		std::cin.ignore();
	}
	return input;
}

/* --==--==--==--== Command ==--===-==--==-- */
inline void Shell::list(){
	action.doViewWorkLogList();
}
inline void Shell::listprev(){
	action.decrementRange();
	action.doViewWorkLogList();
}
inline void Shell::listnext(){
	action.incrementRange();
	action.doViewWorkLogList();
}

inline void Shell::find(){
	std::string keyword = this->getInput<std::string>("keyword");
	if(keyword != "0")
		action.doFindFunction(keyword);
	else
		std::cout << "Aborted." << std::endl;
	std::cout << std::endl;
}
inline void Shell::search(){
	std::string regex = this->getInput<std::string>("keyword");
	if(regex != "0")
		action.doSearchFunction(regex);
	else
		std::cout << "Aborted." << std::endl;
	std::cout << std::endl;
}

inline void Shell::view(){
	int selectId = this->getInput<int>("LogID");
	if(selectId != 0)
		action.doViewWorkLogDetail(selectId);
	else
		std::cout << "Aborted." << std::endl;
	std::cout << std::endl;
}

inline void Shell::edit(){
	action.doEditWorkLog();
}

inline void Shell::reedit(){
	int selectId = this->getInput<int>("LogID");
	if(selectId != 0)
		action.doEditWorkLog(true, selectId);
	else
		std::cout << "Aborted." << std::endl;
	std::cout << std::endl;
}

inline void Shell::remove(){
	int selectId = this->getInput<int>("LogID");
	if(selectId != 0)
		action.doRemoveWorkLog(selectId);
	else
		std::cout << "Aborted." << std::endl;
	std::cout << std::endl;
}

inline void Shell::backup(){
	while(true){
		char yon = this->getInput<char>("y/n");
		if(yon == 'y'){
			action.doBackupWorkLogFile();
			break;
		}else if(yon == 'n'){
			std::cout << ">> Backup Cancelled." << std::endl;
			break;
		}
	}
}

inline void Shell::restore(){
	action.doRestoreWorkLogFile();
}

inline void Shell::ls_backup(){
	action.doLsBackupWorkLogFile();
}

inline void Shell::help(){
	std::cout << std::endl
	          << "[list] or [ls]:"           << "\t\t\t"   << "Preview WorkLog List"                        << std::endl
	          << "[listprev] or [listp]:"    << "\t\t"     << "Preview WorkLog List Previous page"          << std::endl
	          << "[listnext] or [listn]:"    << "\t\t"     << "Preview WorkLog List Next page"              << std::endl
	          << "[find s]:"                 << "\t\t\t"   << "Find Function by keyword(Don't Using Regex)" << std::endl
	          << "[search s]:"               << "\t\t"     << "Search LogData by keyword(Using Regex)"      << std::endl
	          << "[view n]:"                 << "\t\t\t"   << "View WorkLog by given ID"                    << std::endl
	          << "[edit]:"                   << "\t\t\t\t" << "Edit WorkLog"                                << std::endl
	          << "[reedit n] or [revise n]:" << '\t'       << "Re Edit WorkLog by given ID"                 << std::endl
	          << "[remove n]:"               << "\t\t\t"   << "Remove WorkLog by given ID"                  << std::endl
	          << "[q] or [exit]:"            << "\t\t\t"   << "Exiting Shell"                               << std::endl
	          << "[h] or [help]:"            << "\t\t\t"   << "View This Help"                              << std::endl
	          << std::endl;
}

/* --==--==--==--==--=---=--==--==--==--==-- */

