#include "Shell.h"
#include <iostream>
#include <string>
#include "../Lib/StringUtils.hpp"
#include <cstdlib>
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
	"rm_backup" , "rmb"      ,
	"ls_backup" , "lsb"      ,
	"q"         , "exit"     , 
	"h"         , "help"
};

/* --==--==--==--== Run ==--===-==--==-- */
void Shell::run(){
	std::cout << "[help] to viewing help" << std::endl << std::endl;
	while(true){
		std::cout << "awork$ ";
		
		std::string cmd;
		std::cin >> std::ws >> cmd;
		try{
			bool noexit = this->launch( alib::trim(cmd) );
			if(!noexit)  break;
		}catch(std::invalid_argument& e){
			std::cerr << e.what() << ": " << cmd << std::endl;
		}
	}
}


/* --==--==--==--== Launch ==--===-==--==-- */
inline std::vector<int> Shell::vaToVi(std::vector<std::string>& va){
	std::vector<int> vi;
	for(auto& a : va)
		vi.push_back( std::atoi(a.c_str()) );
	return vi;
}
bool Shell::launch(std::string&& cmd) throw(std::invalid_argument){
	try{
		return this->launch(cmd);
	}catch(std::invalid_argument& e){
		throw e;
	}
}
bool Shell::launch(std::string& cmd) throw(std::invalid_argument){
	// 引数とコマンドを分割
	std::vector<std::string> cmdLine = alib::split(cmd, ' ');

	/* 渡された引数が何番目のコマンドか判定 */
	int i;
	for(i=0; i<CMD_LEN; i++)
		if(cmdLine[0] == CMD_LIST[i])
			break;
	if(i == CMD_LEN)
		throw std::invalid_argument("command not found");

	/* 列挙子として判定 */
	cmdLine.erase( cmdLine.begin() );
	bool argFlag = static_cast<bool>( cmdLine.size() );
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
			if(argFlag)  this->find(cmdLine[0]);
			else         this->find();
			break;
		case SEARCH:
			if(argFlag)  this->search(cmdLine[0]);
			else         this->search();
			break;
		case VIEW:
			if(argFlag)  this->view( vaToVi(cmdLine) );
			else         this->view();
			break;
		case EDIT:
		case WRITE:
			this->edit();
			break;
		case REEDIT:
		case REVISE:
			if(argFlag)  this->reedit( std::atoi(cmdLine[0].c_str()) );  // unknown str is 0 => abort;
			else         this->reedit();
			break;
		case REMOVE:
			if(argFlag)  this->remove( vaToVi(cmdLine) );
			else         this->remove();
			break;
		case BACKUP:
			this->backup();
			break;
		case RESTORE:
			if(argFlag)  this->restore( std::atoi(cmdLine[0].c_str()) );
			else         this->restore();
			break;
		case RM_BACKUP:
		case RMB:
			if(argFlag)  this->rm_backup( vaToVi(cmdLine) );
			else         this->rm_backup();
			break;
		case LS_BACKUP:
		case LSB:
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
inline void Shell::find(std::string& arg){
	if(arg != "0")
		action.doFindFunction(arg);
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
inline void Shell::search(std::string& arg){
	if(arg != "0")
		action.doSearchFunction(arg);
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
inline void Shell::view(std::vector<int>&& args){
	for(auto id : args){
		if(id != 0)
			action.doViewWorkLogDetail(id);
		else
			std::cout << "Aborted." << std::endl;
		std::cout << std::endl;
	}
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
inline void Shell::reedit(int arg){
	if(arg != 0)
		action.doEditWorkLog(true, arg);
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
inline void Shell::remove(std::vector<int>&& args){
	for(auto id : args){
		if(id != 0)
			action.doRemoveWorkLog(id);
		else
			std::cout << "Aborted." << std::endl;
		std::cout << std::endl;
	}
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
	int selectId = this->getInput<int>("restore target FileID");
	if(selectId != 0)
		action.doRestoreWorkLogFile(selectId);
	else
		std::cout << "Aborted." << std::endl;
	std::cout << std::endl;
}
inline void Shell::restore(int arg){
	if(arg != 0)
		action.doRestoreWorkLogFile(arg);
	else
		std::cout << "Aborted." << std::endl;
	std::cout << std::endl;
}

inline void Shell::rm_backup(){
	int selectId = this->getInput<int>("remove FileID");
	if(selectId != 0)
		action.doRemoveWorkLogFile(selectId);
	else
		std::cout << "Aborted." << std::endl;
	std::cout << std::endl;
}
inline void Shell::rm_backup(std::vector<int>&& args){
	for(int i=0; i<args.size(); i++){
		args[i] -= i;  // fix cont numbers
		if(args[i] != 0)
			action.doRemoveWorkLogFile(args[i]);
		else
			std::cout << "Aborted." << std::endl;
		std::cout << std::endl;
	}
}

inline void Shell::ls_backup(){
	action.doViewBackupList();
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
	          << "[backup]:"                 << "\t\t\t"   << "Backup Database file"                        << std::endl
	          << "[restore n]:"              << "\t\t\t"   << "Restore Database file from backed up files"  << std::endl
	          << "[rm_backup n] or [rmb n]:" << '\t'       << "Remove Backup file by given fileID"          << std::endl
	          << "[ls_backup] or [lsb]:"     << "\t\t"     << "List up Backup files"                        << std::endl
	          << "[q] or [exit]:"            << "\t\t\t"   << "Exiting Shell"                               << std::endl
	          << "[h] or [help]:"            << "\t\t\t"   << "View This Help"                              << std::endl
	          << std::endl;
}

/* --==--==--==--==--=---=--==--==--==--==-- */

