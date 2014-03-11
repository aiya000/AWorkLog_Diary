#include "ActionControl.h"
#include "../../Database/WorkLogDBHelper.h"
#include "../../Database/WorkLogData.h"
#include "../../Lib/StringUtils.hpp"
#include "../../Lib/SystemUtils.hpp"
#include "../../Database/DBFailureException.h"
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>


/* --==--==--==--==--==--==--==--==--==-- */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
void ActionControl::doWriteWorkLog(){
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
				while(!getFlag) switch( getchar() ){
				case 'r':
					i--;
				case 'c':
					getFlag = true;
					break;
				case 'a':
					std::cout << ">> Operation Aborted" << std::endl;
					return;
				default:
					std::cout << ">> If Continue(c), Retry(r), Abort(a)." << std::endl;
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
	try{
		WorkLogData logData( time(nullptr), texts[0].str(), texts[1].str(), texts[2].str() );
		WorkLogDBHelper dbHelper;
		dbHelper.writeWorkLog(logData);
	}catch(DBFailureException e){
		std::cerr << e.what()                                << std::endl
		          << ">> Writing to Database faild"          << std::endl
		          << ">> Directory Permission is Writable ?" << std::endl;
		return;
	}
	std::cout << std::endl
	          << " --------------------" << std::endl
	          << " >> WorkLog Witten ! " << std::endl
	          << " --------------------" << std::endl;

}
#pragma GCC diagnostic pop

/* --==--==--==--==--==--==--==--==--==-- */

