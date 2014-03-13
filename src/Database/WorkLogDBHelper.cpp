#include "WorkLogDBHelper.h"
#include "../Config/ConfigLoader.h"
#include <iostream>
#include "../Lib/StringUtils.hpp"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cassert>

/* --==--==--==--==--==--==--==--==--==-- */

WorkLogDBHelper::WorkLogDBHelper() throw(DBFailureException) :
	TABLE_NAME("WorkLog"),
	CREATE_TABLE(std::string("") +
		"CREATE TABLE " + TABLE_NAME +
		"(" +
		" id        integer       primary key autoincrement," +
		" time      timestamp     not null," +
		" function  varchar(16)   not null," +
		" target    varchar(128)  not null," +
		" comment   text          not null"  +
		");")
{
	ConfigLoader loader;
	const char* DB_PATH = loader.getDbPath().c_str();

	/* ワークディレクトリ -> ディレクトリ作成 */
	bool dirExist = this->directoryIsFound();
	if( !dirExist ){
		std::cout << alib::format(">> workdirectory initialize(mkdir) [%s]", loader.getWorkDirPath().c_str())
		          << std::endl;
		mkdir(loader.getWorkDirPath().c_str(), 0744);
	}

	int err = sqlite3_open(DB_PATH, &m_con);
	if(err != SQLITE_OK)
		throw DBFailureException(std::string()+"Connection faild["+DB_PATH+"]");

	/* テーブルチェック -> DB初期化 */
	if( !dirExist || !this->tableIsFound() ){
		try{
			this->createTable();
		}catch(DBFailureException e){
			throw e;
		}
	}

	/* ワークログから最初の10個を取得 */
	try{
		m_workLog = loadWorkLogByIndex(0, 9);
	}catch(DBFailureException e){
		throw e;
	}
}

WorkLogDBHelper::~WorkLogDBHelper(){
	this->close();
}
void WorkLogDBHelper::close(){
	sqlite3_close(m_con);
}

/* --==--==--==--==--==--==--==--==--==-- */


/**-**** Private ****-**/

bool WorkLogDBHelper::directoryIsFound(){
	ConfigLoader dir;
	const std::string WORK_PATH = dir.getWorkDirPath();
	if( !opendir(WORK_PATH.c_str()) ){
		return false;
	}else{
		return true;
	}
}

bool WorkLogDBHelper::tableIsFound(){
	/* ワークディレクトリのチェック */
	if( !this->directoryIsFound() )
		return false;

	/* テーブルチェック */
	const std::string TABLE_EXIST_SQL = std::string() +
		"SELECT count(*) FROM sqlite_master" +
		" WHERE type = 'table'" +
		" AND   name = '" + TABLE_NAME + "';";
	sqlite3_stmt *stmt;
	sqlite3_prepare(m_con, TABLE_EXIST_SQL.c_str(), TABLE_EXIST_SQL.size(), &stmt, nullptr);
	sqlite3_reset(stmt);
	sqlite3_step(stmt);

	if( sqlite3_column_int(stmt, 0) == 0 ){  
		sqlite3_finalize(stmt);
		return false;
	}else{
		sqlite3_finalize(stmt);
		return true;
	}
}

void WorkLogDBHelper::createTable() throw(DBFailureException){
	sqlite3_stmt *stmt;
	sqlite3_prepare(m_con, CREATE_TABLE.c_str(), CREATE_TABLE.size(), &stmt, nullptr);
	sqlite3_reset(stmt);
	// プロセッシング
	for(int i=0; sqlite3_step(stmt) != SQLITE_DONE; i++)
		if(i > 1000)  throw DBFailureException("Create Table SQL TimeOut");

	sqlite3_finalize(stmt);
}

/* -=-=-=-=-=- */

std::vector<WorkLogData> WorkLogDBHelper::loadWorkLogByIndex(int startIndex, int endIndex)
	throw(DBFailureException)
{
	/*
	 * TODO:現在全体取得になっています。
	 */
	std::vector<WorkLogData> workLog;
	const std::string SQL = std::string() +
		"SELECT id, time, function, target, comment" +
		" FROM " + TABLE_NAME + ";";
	sqlite3_stmt* stmt;
	sqlite3_prepare(m_con, SQL.c_str(), SQL.size(), &stmt, nullptr);
	sqlite3_reset(stmt);

	int r;
	while( (r = sqlite3_step(stmt)) == SQLITE_ROW ){
		// sqlite3_column_textの戻り値はなぜかconst unsigned char*...。
		WorkLogData rowData(
				sqlite3_column_int(stmt, 0),  // id
				sqlite3_column_int(stmt, 1),  // time
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)), // function
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)), // target
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))  // comment
				);
		workLog.push_back(rowData);
	}
	if(r != SQLITE_DONE)
		throw DBFailureException("WorkLog Select Error");
	/*
	 * クエリをインターバルごとに読み取り
	 * startIndex から endIndex-1 まで
	 */
	return workLog;
}


/**-**** Public ****-**/

std::vector<WorkLogData>& WorkLogDBHelper::getWorkLog(){
	return m_workLog;
}
WorkLogData& WorkLogDBHelper::getWorkLogByIndex(int index){
	return m_workLog[index];
}

void WorkLogDBHelper::writeWorkLog(WorkLogData& values) throw(DBFailureException){
	if(values.getId() != -1)
		throw DBFailureException("内部エラー: 新規DB書き込みの場合はIDが-1のはずです。");

	sqlite3_stmt* stmt;
	const std::string INSERT_SQL = std::string() +
		"INSERT INTO " + TABLE_NAME + "(time, function, target, comment)" + 
		" VALUES(?, ?, ?, ?);";
	sqlite3_prepare(m_con, INSERT_SQL.c_str(), INSERT_SQL.size(), &stmt, nullptr);
	sqlite3_reset(stmt);
	sqlite3_bind_int(stmt,  1, values.getTime());
	sqlite3_bind_text(stmt, 2, values.getFunction().c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, values.getTarget().c_str(),   -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 4, values.getComment().c_str(),  -1, SQLITE_TRANSIENT);

	for(int i=0; sqlite3_step(stmt) != SQLITE_DONE; i++)
		if(i > 1000)  throw DBFailureException("Insert SQL TimeOut");
	sqlite3_finalize(stmt);
}

void WorkLogDBHelper::updateWorkLog(WorkLogData& values) throw(DBFailureException){
	if(values.getId() == -1)
		throw DBFailureException("内部エラー: 既存データ書き換えの場合はIDが-1ではないはずです。");

	sqlite3_stmt* stmt;
	const std::string UPDATE_SQL = std::string() +
		"UPDATE " + TABLE_NAME +
		" SET " + "function='?',target='?',comment='?'"  +
		" WHERE id=?;";
	sqlite3_prepare(m_con, UPDATE_SQL.c_str(), UPDATE_SQL.size(), &stmt, nullptr);
	sqlite3_reset(stmt);
	sqlite3_bind_text(stmt, 1, values.getFunction().c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, values.getTarget().c_str(),   -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, values.getComment().c_str(),  -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,  4, values.getId());

	for(int i=0; sqlite3_step(stmt) != SQLITE_DONE; i++)
		if(i > 1000)  throw DBFailureException("Update SQL TimeOut");
	sqlite3_finalize(stmt);
}

/* --==--==--==--==--==--==--==--==--==-- */

