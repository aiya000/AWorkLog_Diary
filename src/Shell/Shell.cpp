#include <ncurses.h>
#include "Shell.h"
#include <cassert>
#include <iostream>
#include <string>
#include "../Lib/StringUtils.hpp"

const std::string Select::CMD_LIST[] = {
	"list"   , "ls"     ,
	"view"   , 
	"edit"   , "write"  , 
	"reedit" , "revise" , 
	"remove" , 
	"q"      , "exit"   , 
	"h"      , "help"
};

/* --==--==--==--== Run ==--===-==--==-- */
void Select::run(){
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
bool Select::launch(std::string&& cmd) throw(std::invalid_argument){

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

		case Q:
		case EXIT:
			return false;

		case H:
		case HELP:
			this->help();
			break;

		default:
			assert(!"Select#launch -> invalid case");
	}

	return true;
}


/* --==--==--==--== Util ==--===-==--==-- */
inline int Select::getInput(){
	std::cout << "Please Input LogID (abort: 0)" << std::endl << ">> ";
	int selectId;
	std::cin >> selectId;
	return selectId;
}

/* --==--==--==--== Command ==--===-==--==-- */
inline void Select::list(){
	action.doViewWorkLogList();
}

inline void Select::view(){
	int selectId = this->getInput();
	if(selectId != 0)
		action.doViewWorkLogDetail(selectId);
	else
		std::cout << "Aborted" << std::endl;
	std::cout << std::endl;
}

inline void Select::edit(){
	action.doWriteWorkLog();
}

inline void Select::reedit(){
	int selectId = this->getInput();
	if(selectId != 0)
		action.doWriteWorkLog(true, selectId);
	else
		std::cout << "Aborted" << std::endl;
	std::cout << std::endl;
}

inline void Select::remove(){
	int selectId = this->getInput();
	if(selectId != 0)
		action.doRemoveWorkLog(selectId);
	else
		std::cout << "Aborted" << std::endl;
	std::cout << std::endl;
}

inline void Select::help(){
	std::cout << std::endl
	           << "[view n]:"                 << "\t\t\t" << "Worklog View by given ID"     << std::endl
	           << "[edit]:"                   << "\t\t\t\t" << "Worklog Edit"                 << std::endl
	           << "[reedit n] or [revise n]:" << "\t"     << "Re Edit WorkLog by given ID"  << std::endl
	           << "[remove n]:"               << "\t\t\t"   << "Remove Worklog by given ID"   << std::endl
	           << "[q] or [exit]:"            << "\t\t\t"   << "Exiting Shell"                << std::endl
	           << "[h] or [help]:"            << "\t\t\t"   << "View This Help"               << std::endl
	           << std::endl;
}

/* --==--==--==--==--=---=--==--==--==--==-- */

