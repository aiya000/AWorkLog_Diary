#include "ActionControl.h"
#include "../../Lib/Convert.hpp"
#include "../../Database/WorkLogDBHelper.h"
#include "../../Database/WorkLogData.h"
#include "../../Lib/StringUtils.hpp"
#include "../../Lib/StreamUtils.hpp"
#include "../../Lib/SystemUtils.hpp"
#include "../../Database/DBFailureException.h"
#include <vector>
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <cassert>

/* --==--==--==--==--==--==--==--==--==-- */
ActionControl::ActionControl() :
	m_range({0, [&](){
		const int LOG_SIZE = m_dbHelper.getWorkLogSize();
		const int LOAD_RANGE = 20;
		return
		LOG_SIZE >= LOAD_RANGE?  LOAD_RANGE
		                      :  LOG_SIZE;
	}()}){}

/* --==--==--==--==--==--==--==--==--==-- */
bool ActionControl::confirm(){
	while(true){
		std::cout << "Apply Operation, OK? (y/n)" << std::endl;
		char confirm;
		std::cin >> confirm;
		if(confirm == 'y' || confirm == 'Y')
			return true;
		else if(confirm == 'n' || confirm == 'N')
			return false;
		std::cout << "Please [y] or [n]" << std::endl;
	}
	std::cout << std::endl;
}

void ActionControl::viewDetails(WorkLogData workLog, bool viewComment){
		std::cout << " --------------------"                                 << std::endl
		          << " Time:\t\t"     << alib::timeToString(workLog.getTime()) << std::endl
		          << " Function:\t" << workLog.getFunction()                 << std::endl
		          << " Target:\t"   << workLog.getTarget()                   << std::endl;
		if(viewComment){
			std::cout << " Comment: " << std::endl;
			for(std::string line : alib::split(workLog.getComment(), '\n'))
				std::cout << " % " << line << std::endl;
		}
		std::cout << " --------------------"                        << std::endl;
}
void ActionControl::viewColumn(WorkLogData workLog){
	std::cout << "|"
	          << workLog.getId()                       << "\t|"
	          << alib::timeToString(workLog.getTime()) << "\t|"
	          << workLog.getFunction()                 << "\t|"
	          << workLog.getTarget()                   << "\t|"
	          << std::endl;
}

/* --==--==--==--==--==--==--==--==--==-- */
void ActionControl::incrementRange(){
	const int ROW_COUNT = m_dbHelper.getWorkLogSize();
	const int DEFAULT  = 20;
	const int LOAD_NUM =  // data under 20 case
		ROW_COUNT >= DEFAULT?  DEFAULT
		                    :        0;

	m_range.start += LOAD_NUM;
	m_range.end   += LOAD_NUM;
	if(m_range.end > ROW_COUNT){
		m_range.end   = ROW_COUNT;
		m_range.start = ROW_COUNT - DEFAULT;
	}
}
void ActionControl::decrementRange(){
	const int ROW_COUNT = m_dbHelper.getWorkLogSize();
	const int LOAD_NUM = 20;
	m_range.start -= LOAD_NUM;
	m_range.end   -= LOAD_NUM;
	if(m_range.start < 0){
		m_range.start = 0;
		m_range.end   =
			ROW_COUNT >= 20?  20
			               :  ROW_COUNT;
	}
}

/* --==--==--==--==--==--==--==--==--==-- */
void ActionControl::doViewWorkLogList(){
	std::vector<WorkLogData> workLog = m_dbHelper.getWorkLogByRange(m_range.start, m_range.end);
	for(int i=0; i<workLog.size(); i++)
		this->viewColumn(workLog[i]);
}

void ActionControl::doFindFunction(std::string keyword){
	std::vector<WorkLogData> workLog = m_dbHelper.getWorkLogFindByKeyword(keyword);
	for(int i=0; i<workLog.size(); i++)
		this->viewColumn(workLog[i]);
}
void ActionControl::doSearchFunction(std::string regex){
	std::vector<WorkLogData> workLog = m_dbHelper.getWorkLogSearchByRegex(regex);
	for(int i=0; i<workLog.size(); i++)
		this->viewColumn(workLog[i]);
}

void ActionControl::doViewWorkLogDetail(int id){
	int i;
	try{
		WorkLogData workLog = m_dbHelper.getWorkLogById(id);
		this->viewDetails(workLog, true);
	}catch(DBFailureException e){
		std::cerr << ">> No such WorkLog ID: " << id << std::endl;
	}
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
void ActionControl::doEditWorkLog(bool reeditFlag, int id){
	/* 乱数でテンポラリファイル名を作成 */
	constexpr int FILE_NUM = 3;
	std::string fileList[FILE_NUM];
	/*
	 * Function
	 * Target
	 * Comment
	 */
	std::random_device rand;
	for(int i=0; i<FILE_NUM; i++){
		std::stringstream ss;
		ss << "/tmp/" << rand();
		fileList[i] = ss.str();
	}

	/* 再編集モードならあらかじめテキストの内容をセットしておく */
	if(reeditFlag){
		try{
			WorkLogData workLog = m_dbHelper.getWorkLogById(id);
			std::ofstream fout(fileList[0], std::ios::out);
			fout << workLog.getFunction();
			fout.close();  fout.open(fileList[1], std::ios::out);
			fout << workLog.getTarget();
			fout.close();  fout.open(fileList[2], std::ios::out);
			fout << workLog.getComment();
		}catch(DBFailureException e){
			std::cerr << ">> No such WorkLog ID: " << id << std::endl;
			return;
		}
	}

	/* 処理、対象、コメントを読み込み */
	std::tr1::array<std::string,3> texts = 
		this->editDetailsUseStdin(fileList[2], reeditFlag, id);
		//this->editDetails(fileList[0], fileList[1], fileList[2]);

	/* ファイル削除 */
	for(std::string file : fileList)
		std::remove(file.c_str());

	/* データベースに書き込み */
	WorkLogData logData( time(nullptr), texts[0], texts[1], texts[2] );
	this->viewDetails(logData, true);
	if(this->confirm()){
		try{
			if(!reeditFlag){
				m_dbHelper.writeWorkLog(logData);
			}else{
				logData.setId(id);
				m_dbHelper.updateWorkLog(logData);
			}
		}catch(DBFailureException e){
			std::cerr << e.what()                                             << std::endl
			          << ">> Writing to Database faild"                       << std::endl
			          << ">> Directory Permission is Readable and Writable ?" << std::endl;
			return;
		}
		std::cout << std::endl
		          << " --------------------" << std::endl
		          << " >> WorkLog Witten ! " << std::endl
		          << " --------------------" << std::endl;
	}else{
		std::cout << " >> Witten Cancelled! " << std::endl;
	}
}
#pragma GCC diagnostic pop
#pragma GCC diagnostic push

void ActionControl::doRemoveWorkLog(int id){
	try{
		WorkLogData workLog = m_dbHelper.getWorkLogById(id);
		this->viewDetails(workLog);
	}catch(DBFailureException e){
		std::cerr << ">> No such WorkLog ID: " << id << std::endl;
		return;
	}

	std::cout << ">> Remove This WorkLog." << std::endl;
	try{
		if(this->confirm())
			m_dbHelper.removeWorkLog(id);
	}catch(DBFailureException e){
		std::cerr << e.what() << std::endl;
	}
}

/* --==--==--==--==--==--==--==--==--==-- */
std::tr1::array<std::string,3> ActionControl::editDetails(/*{{{*/
		const std::string& funcPath,
		const std::string& tarPath,
		const std::string& comPath)
	throw(alib::SystemCommandCallException, alib::SystemInterruptedException)
{
	std::tr1::array<std::string,3> details;
	constexpr int FILE_NUM = 3;
	std::string fileList[] = { funcPath, tarPath, comPath };

	/* シェル変数読み込み */
	std::string editor = std::getenv("EDITOR");

	for(int i=0; i<FILE_NUM; i++){
		std::stringstream detail;

		// Commentのみ処理を特殊化
		int err = system( (editor+" "+fileList[i]).c_str() );
		if(err == -1){
			throw alib::SystemCommandCallException(
					">> Opening $EDITOR faild"
					">> Do you have an editor ?");
		}else{
			std::ifstream fin(fileList[i], std::ios::in);
			std::string line;

			try{
				// Commentのみ処理を特殊化
				if(i != 2){
					fin >> line;
					detail << line;
					if(line == "")  throw DBFailureException("Text is Empty");
				}else{
					while( std::getline(fin, line) )
						detail << line << std::endl;
					if(line == "")
						std::cout << "Warn>> Comment is Empty" << std::endl;
				}
			}catch(DBFailureException e){
				std::cout << ">> " << e.what() << std::endl;
				std::cout << ">> If Continue(c), Retry(r), Abort(a)." << std::endl;

				bool getFlag = false;
				char a;
				while(!getFlag){
					std::cin >> a;
					switch(a){
						case 'r':
							i--;
						case 'c':
							getFlag = true;
							break;
						case 'a':
							throw alib::SystemInterruptedException(">> Operation Aborted");
						default:
							std::cout << ">> Please [c] or [r] or [a]." << std::endl;
							break;
					}
					continue;
				}

			}
		}

		details[i] = detail.str();
	}
	return details;
}/*}}}*/
std::tr1::array<std::string,3> ActionControl::editDetailsUseStdin(/*{{{*/
		const std::string& comPath,
		bool reeditFlag,
		int  id)
	throw(alib::SystemCommandCallException, alib::SystemInterruptedException)
{
	std::tr1::array<std::string,3> details;
	constexpr int FILE_NUM = 3;

	/* ReEditモードのみで使うデータ */
	WorkLogData *oldData;
	if(reeditFlag)  oldData = &m_dbHelper.getWorkLogById(id);

	std::pair<std::string,std::string> entry[] = {
		std::pair<std::string,std::string>("Function Type", (reeditFlag)? oldData->getFunction(): ""),
		std::pair<std::string,std::string>("Target Name",   (reeditFlag)? oldData->getTarget():   "")
	};

	std::cin.ignore();  // flush (<- getline)
	for(int i=0; i<FILE_NUM; i++){
		std::stringstream detail;
		// Commentのみ処理を特殊化
		if(i != 2){
			if(reeditFlag)
				std::cout << "Do not change by Enter key" << std::endl;
			std::cout << entry[i].first;
			if(reeditFlag)
				std::cout << "(default " << entry[i].second << ')';
			std::cout << ">> ";

			std::string input;
			bool inputFlag = false;
			while(!inputFlag){
				std::getline(std::cin, input);
				if(input != ""){
					inputFlag = true;
				}else if(reeditFlag && input == ""){
					input = entry[i].second;
					inputFlag = true;
				}
			}
			detail << input;

		}else{
			/* シェル変数読み込み */
			std::string editor = std::getenv("EDITOR");

			int err = system( (editor+" "+comPath).c_str() );
			if(err == -1){
				throw alib::SystemCommandCallException(
						">> Opening $EDITOR faild"
						">> Do you have an editor ?");
			}else{
				std::ifstream fin(comPath, std::ios::in);
				std::string line;

				try{
					while( std::getline(fin, line) )
						detail << line << std::endl;
					if(line == "")
						std::cout << "Warn>> Comment is Empty" << std::endl;
				}catch(DBFailureException e){
					std::cout << ">> " << e.what() << std::endl;
					std::cout << ">> If Continue(c), Retry(r), Abort(a)." << std::endl;

					bool getFlag = false;
					char a;
					while(!getFlag){
						std::cin >> a;
						switch(a){
							case 'r':
								i--;
							case 'c':
								getFlag = true;
								break;
							case 'a':
								throw alib::SystemInterruptedException(">> Operation Aborted");
							default:
								std::cout << ">> Please [c] or [r] or [a]." << std::endl;
								break;
						}
						continue;
					}
				}

			}
		}

		details[i] = detail.str();
	}
	return details;
}/*}}}*/

/* --==--==--==--==--==--==--==--==--==-- */
