#include <ncurses.h>
#include "Select.h"
#include <cassert>
#include <iostream>
#include <string>
#include "../Lib/StringUtils.hpp"

const std::string Select::CMD_LIST[] = {
	"view",
	"edit",
	"remove",
	"q",
	"exit",
	"h",
	"help"
};

bool Select::launch(std::string&& cmd) throw(std::invalid_argument){
	int i;
	for(i=0; i<CMD_LEN; i++)
		if(cmd == CMD_LIST[i])
			break;
	if(i == CMD_LEN)
		throw std::invalid_argument("command not found");

	switch(i){
		case 0:
			break;
		case 1:
			action.doWriteWorkLog();
			return false;
		case 2:
			break;

		case 3:
		case 4:
			// exit;
			return false;

		case 5:
		case 6:
			std::cout << std::endl
			          << "[view]:\tViewing Worklog"                   << std::endl
			          << "[edit]:\tEditing Worklog"                   << std::endl
			          << "[remove]:\tSelecting and Removinng Worklog" << std::endl
			          << "[q] or [exit]:\tExiting Shell"              << std::endl
			          << "[h] or [help]:\tViewing This Help"          << std::endl
			          << std::endl;
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

