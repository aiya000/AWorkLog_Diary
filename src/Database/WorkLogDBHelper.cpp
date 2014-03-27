#include "WorkLogDBHelper.h"
#include "../Config/ConfigLoader.h"
#include <iostream>
#include "../Lib/StringUtils.hpp"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cassert>
#include <memory>

/* --==--==--==--==--==--==--==--==--==-- */

WorkLogDBHelper::WorkLogDBHelper() throw(DBFailureException) :
	TABLE_NAME("WorkLog"),
	CREATE_TABLE(
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
	const std::string DB_PATH = loader.getDbPath();

	/* ワークディレクトリ -> ディレクトリ作成 */
	bool dirExist = this->directoryIsFound();
	if( !dirExist ){
		std::cout << alib::format(">> workdirectory initialize(mkdir) [%s]", loader.getWorkDirPath().c_str())
		          << std::endl;
		mkdir(loader.getWorkDirPath().c_str(), 0744);
	}

	int err = sqlite3_open(DB_PATH.c_str(), &m_con);
	if(err != SQLITE_OK)
		throw DBFailureException("Connection faild["+DB_PATH+"]");

	/* テーブルチェック -> DB初期化 */
	if( !dirExist || !this->tableIsFound() ){
		try{
			this->createTable();
		}catch(DBFailureException e){
			sqlite3_close(m_con);
			throw e;
		}
	}

	/* DBをオンメモリ化 */
	this->loadWorkLog();
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
	if( !opendir(WORK_PATH.c_str()) )
		return false;
	else
		return true;
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

void WorkLogDBHelper::loadWorkLog() throw(DBFailureException){
	const std::string SQL = std::string() +
		"SELECT id, time, function, target, comment" +
		" FROM " + TABLE_NAME +
		" ORDER BY time desc;";
	sqlite3_stmt* stmt;
	sqlite3_prepare(m_con, SQL.c_str(), SQL.size(), &stmt, nullptr);
	sqlite3_reset(stmt);

	int r;
	while( (r = sqlite3_step(stmt)) == SQLITE_ROW ){
		// sqlite3_column_textの戻り値はなぜかconst unsigned char*なのでキャスト。
		WorkLogData rowData(
				sqlite3_column_int(stmt, 0),  // id
				sqlite3_column_int(stmt, 1),  // time
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)), // function
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)), // target
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))  // comment
				);
		m_workLog.push_back(rowData);
	}
	if(r != SQLITE_DONE)
		throw DBFailureException("Select SQL Failed");
	sqlite3_finalize(stmt);
}

void WorkLogDBHelper::refreshWorkLogContainer()
	throw(DBFailureException)
{
	try{
		this->loadWorkLog();
		this->getWorkLogByRange(0, this->getWorkLogSize());
	}catch(DBFailureException e){
		throw DBFailureException(e.what() + std::string("(Refresh Error)"));
	}
}


/**-**** Public ****-**/

std::vector<WorkLogData>& WorkLogDBHelper::getWorkLog(){
	return m_workLog;
}
std::vector<WorkLogData>& WorkLogDBHelper::getWorkLogByRange(int start, int end){
	// m_workLog.begin()+start+LOAD_NUMがm_workLog.end()を超えないことは
	// ActionControl#incrementIndex()で保証済み
	m_selectedWorkLog.clear();

	std::copy(m_workLog.begin()+start, m_workLog.begin()+end,
			std::back_inserter(m_selectedWorkLog));
	return m_selectedWorkLog;
}
WorkLogData& WorkLogDBHelper::getWorkLogById(int id) throw(DBFailureException){
	for(int i=0; i<m_workLog.size(); i++)
		if(id == m_workLog[i].getId())
			return m_workLog[i];
	throw DBFailureException("Not Found Database Data, Search By ID");
}
std::vector<WorkLogData>& WorkLogDBHelper::getWorkLogFindByKeyword(std::string keyword) throw(DBFailureException){
	m_selectedWorkLog.clear();
	for(int i=0; i<m_workLog.size(); i++){
		std::vector<std::string> selection;
		for(std::string& trim : alib::split(m_workLog[i].getFunction(), ','))
			selection.push_back( alib::trim(trim) );

		for(std::string& select : selection)  if(select == keyword){
			m_selectedWorkLog.push_back( m_workLog[i] );
			break;
		}
	}
	if(m_selectedWorkLog.size() == 0)
		throw DBFailureException("Not Found Database Data, Find By Keyword");
	return m_selectedWorkLog;
}
std::vector<WorkLogData>& WorkLogDBHelper::getWorkLogSearchByRegex(std::string regex) throw(DBFailureException, std::regex_error){
	std::unique_ptr<std::regex> searchEx;
	try{
		searchEx.reset(new std::regex(regex));
	}catch(std::regex_error e){
		throw e;
	}

	m_selectedWorkLog.clear();
	for(WorkLogData data : m_workLog)
		if(std::regex_search(data.getFunction().c_str(), *searchEx) ||
		   std::regex_search(data.getTarget().c_str(),   *searchEx) ||
		   std::regex_search(data.getComment().c_str(),  *searchEx))
			m_selectedWorkLog.push_back(data);
	if(m_selectedWorkLog.size() == 0)
		throw DBFailureException("Not Found Database Data, Search By Regex");
	return m_selectedWorkLog;
}


int WorkLogDBHelper::getWorkLogSize(){
	return m_workLog.size();
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

	try{
		this->refreshWorkLogContainer();
	}catch(DBFailureException e){
		throw e;
	}
}

void WorkLogDBHelper::updateWorkLog(WorkLogData& values) throw(DBFailureException){
	if(values.getId() == -1)
		throw DBFailureException("内部エラー: 既存データ書き換えの場合はIDが-1ではないはずです。");

	sqlite3_stmt* stmt;
	const std::string UPDATE_SQL =
		"UPDATE " + TABLE_NAME +
		" SET " + "function=?,target=?,comment=?"  +
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

	try{
		this->refreshWorkLogContainer();
	}catch(DBFailureException e){
		throw e;
	}
}

void WorkLogDBHelper::removeWorkLog(int id) throw(DBFailureException){
	if(id < 0)
		throw DBFailureException("不明なID指定です");

	sqlite3_stmt* stmt;
	const std::string DELETE_SQL =
		"DELETE FROM " + TABLE_NAME +
		" WHERE id=?;";
	sqlite3_prepare(m_con, DELETE_SQL.c_str(), DELETE_SQL.size(), &stmt, nullptr);
	sqlite3_reset(stmt);
	sqlite3_bind_int(stmt, 1, id);

	for(int i=0; sqlite3_step(stmt) != SQLITE_DONE; i++)
		if(i > 1000)  throw DBFailureException("Delete SQL TimeOut");
	sqlite3_finalize(stmt);

	try{
		this->refreshWorkLogContainer();
	}catch(DBFailureException e){
		throw e;
	}
}

/* --==--==--==--==--==--==--==--==--==-- */

