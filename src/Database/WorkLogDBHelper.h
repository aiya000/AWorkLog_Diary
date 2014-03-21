#pragma once
#include "DBFailureException.h"
#include "WorkLogData.h"
#include <vector>
#include <sqlite3.h>

class WorkLogDBHelper {
private:
	std::vector<WorkLogData> m_workLog;
	const std::string TABLE_NAME;
	const std::string CREATE_TABLE;
	sqlite3 *m_con;
public:
	static const int LOAD_NUM = 20;

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
	std::vector<WorkLogData> loadWorkLogByIndex(int startIndex, int endIndex);
	void refreshWorkLogContainer();  // -> m_workLog;

public:
	std::vector<WorkLogData>& getWorkLog();
	std::vector<WorkLogData>& getWorkLogByStartIndex(int start);
	WorkLogData& getWorkLogSearchById(int id) throw(DBFailureException);

	int getRowCount();

	void writeWorkLog(WorkLogData& values)    throw(DBFailureException);
	void updateWorkLog(WorkLogData& values)   throw(DBFailureException);
	void removeWorkLog(int id)  throw(DBFailureException);
};
