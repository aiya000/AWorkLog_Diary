#pragma once
#include "DBFailureException.h"
#include "WorkLogData.h"
#include <vector>
#include <tr1/array>
#include <sqlite3.h>
#include <regex>

class WorkLogDBHelper {
private:
	std::vector<WorkLogData> m_workLog;
	std::vector<WorkLogData> m_selectedWorkLog;  // 参照渡し用,,,This have a part of m_workLog!
	const std::string TABLE_NAME;
	const std::string CREATE_TABLE;
	sqlite3 *m_con;

public:
	WorkLogDBHelper() throw(DBFailureException);
	~WorkLogDBHelper();
	void close();

private:
	/* -=-=-= Use by Constructor Only =-=-=- */

	// ファイル格納ディレクトリの存在確認
	bool directoryIsFound();
	// データ格納テーブルの存在確認
	bool tableIsFound();

	void createTable() throw(DBFailureException);

	/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
	void loadWorkLog() throw(DBFailureException);
	void refreshWorkLogContainer() throw(DBFailureException);  // -> m_workLog;

public:
	std::vector<WorkLogData>& getWorkLog();
	std::vector<WorkLogData>& getWorkLogByRange(int start, int end);
	WorkLogData& getWorkLogById(int id)    throw(DBFailureException);
	std::vector<WorkLogData>& getWorkLogFindByKeyword(std::string keyword)
		throw(DBFailureException);
	std::vector<WorkLogData>& getWorkLogSearchByRegex(std::string regex)
		throw(DBFailureException, std::regex_error);

	int getWorkLogSize();

	void writeWorkLog(WorkLogData& values)    throw(DBFailureException);
	void updateWorkLog(WorkLogData& values)   throw(DBFailureException);
	void removeWorkLog(int id)  throw(DBFailureException);
};
