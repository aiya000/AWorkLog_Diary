#include <ncurses.h>
#include "Select.h"
#include <cassert>
#include <iostream>
#include <string>
#include "../Lib/StringUtils.hpp"

const std::string Select::CMD_LIST[] = {
	"view"   , 
	"edit"   , "write"  , 
	"reedit" , "revise" , 
	"remove" , 
	"q"      , "exit"   , 
	"h"      , "help"
};
inline void Select::usage(){
	std::cout << std::endl
	           << "[view]:"               << "\t\t\t" << "Viewing Worklog"                 << std::endl
	           << "[edit]:"               << "\t\t\t" << "Editing Worklog"                 << std::endl
	           << "[reedit] or [revise]:" << "\t"     << "Re Editing WorkLog"              << std::endl
	           << "[remove]:"             << "\t\t"   << "Selecting and Removinng Worklog" << std::endl
	           << "[q] or [exit]:"        << "\t\t" << "Exiting Shell"                   << std::endl
	           << "[h] or [help]:"        << "\t\t" << "Viewing This Help"               << std::endl
	           << std::endl;
}


bool Select::launch(std::string&& cmd) throw(std::invalid_argument){
	int i;
	for(i=0; i<CMD_LEN; i++)
		if(cmd == CMD_LIST[i])
			break;
	if(i == CMD_LEN)
		throw std::invalid_argument("command not found");

	switch( static_cast<CmdEnum>(i) ){
		case VIEW:
			action.doViewWorkLogList();
			break;
		case EDIT:
		case WRITE:
			action.doWriteWorkLog();
			return false;
		case REEDIT:
		case REVISE:
			std::cout << "Please Input LogID (abort: 0)" << std::endl << ">> ";
			int selectId;
			std::cin >> selectId;
			if(selectId != 0)
				action.doWriteWorkLog(true, selectId);
			else
				std::cout << "Aborted" << std::endl;
			std::cout << std::endl;

			// フラッシュの不具合によるダミー処理
			{std::string dummy;  getline(std::cin, dummy);}
			break;

		case REMOVE:
			break;

		case Q:
		case EXIT:
			return false;

		case H:
		case HELP:
			usage();
			break;

		default:
			assert(!"Select#launch -> invalid case");
	}

	return true;
}

void Select::run(){
	std::cout << "[help] to viewing help" << std::endl << std::endl;
	while(true){
		std::cout << "awork$ ";
		
		std::string cmd;  getline(std::cin, cmd);
		try{
			bool noexit = this->launch( alib::trim(cmd) );
			if(!noexit)  break;
		}catch(std::invalid_argument e){
			std::cerr << e.what() << ": " << cmd << std::endl;
		}
	}
}

