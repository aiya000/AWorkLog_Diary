#include "ActionControl.h"
#include "../../Database/WorkLogDBHelper.h"
#include "../../Database/WorkLogData.h"
#include "../../Lib/StringUtils.hpp"
#include "../../Lib/SystemUtils.hpp"
#include "../../Database/DBFailureException.h"
#include <vector>
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

/* --==--==--==--==--==--==--==--==--==-- */
bool ActionControl::confirm(){
	while(true){
		std::cout << "Do write Database, OK? (y/n)" << std::endl;
		char confirm = std::cin.get();
		if(confirm == 'y' || confirm == 'Y')
			return true;
		else if(confirm == 'n' || confirm == 'N')
			return false;
		std::cout << "Please [y] or [n]" << std::endl;
	}
	std::cout << std::endl;
}

/* --==--==--==--==--==--==--==--==--==-- */

void ActionControl::doViewWorkLogList(){
	std::vector<WorkLogData> workLog = m_dbHelper.getWorkLog();
	for(int i=0; i<workLog.size(); i++){
		std::cout << "|"
		          << workLog[i].getId()       << "\t|"
		          << workLog[i].getTime()     << "\t|"
		          << workLog[i].getFunction() << "\t|"
		          << workLog[i].getTarget()   << "\t|"
		          << std::endl;
	}
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
void ActionControl::doWriteWorkLog(bool reeditFlag, int id){
	/* シェル変数読み込み */
	std::string editor = std::getenv("EDITOR");

	/* 乱数でテンポラリファイル名を作成 */
	std::string fileList[3];
	const int FILE_NUM = 3;
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
		std::vector<WorkLogData> workLog = m_dbHelper.getWorkLog();
		int i;
		for(i=0; i<workLog.size(); i++)  if(workLog[i].getId() == id){
			std::ofstream fout(fileList[0], std::ios::out);
			fout << workLog[i].getFunction();
			fout.close();  fout.open(fileList[1], std::ios::out);
			fout << workLog[i].getTarget();
			fout.close();  fout.open(fileList[2], std::ios::out);
			fout << workLog[i].getComment();
			break;
		}
		if(i == workLog.size()){
			std::cerr << ">> No such WorkLog ID: " << id << std::endl;;
			return;
		}
	}

	/* 処理、対象、コメントを読み込み */
	std::stringstream texts[3];
	for(int i=0; i<FILE_NUM; i++){
		if( system( (editor+" "+fileList[i]).c_str() ) == -1 ){
			std::cerr << ">> Opening $EDITOR faild"   << std::endl
				<< ">> Do you have an editor ?" << std::endl;
			return;
		}else{
			std::ifstream fin(fileList[i], std::ios::in);
			std::string line;

			try{
				// Commentのみ処理を特殊化
				if(i != 2){
					std::getline(fin, line);
					texts[i] << line;
					if(texts[i].str() == "")  throw DBFailureException("Text is Empty");
				}else while( std::getline(fin, line) ){
					texts[i] << line << std::endl;
					std::cout << "Warn>> Comment is Empty" << std::endl;
				}
			}catch(DBFailureException e){
				std::cout << ">> " << e.what() << std::endl;
				std::cout << ">> If Continue(c), Retry(r), Abort(a)." << std::endl;

				bool getFlag = false;
				while(!getFlag) switch( std::cin.get() ){
				case 'r':
					i--;
				case 'c':
					getFlag = true;
					break;
				case 'a':
					std::cout << ">> Operation Aborted" << std::endl;
					return;
				default:
					std::cout << ">> Please [c] or [r] or [a]." << std::endl;
					break;
				}
				continue;

			}
		}
	}

	/* ファイル削除 */
	for(std::string file : fileList)
		std::remove(file.c_str());

	/* データベースに書き込み */
	if(this->confirm()){
		try{
			if(!reeditFlag){
				WorkLogData logData( time(nullptr), texts[0].str(), texts[1].str(), texts[2].str() );
				m_dbHelper.writeWorkLog(logData);
			}else{
				WorkLogData logData( id, time(nullptr), texts[0].str(), texts[1].str(), texts[2].str() );
				m_dbHelper.updateWorkLog(logData);
			}
		}catch(DBFailureException e){
			std::cerr << e.what()                                       << std::endl
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

/* --==--==--==--==--==--==--==--==--==-- */

