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
	WorkLogDBHelper() throw(DBFailureException);
	~WorkLogDBHelper();
	void close();

private:
	/* -=-=-= Use by Constructor Only =-=-=- */

	// ファイル格納ディレクトリの存在確認
	bool directoryIsFound();
	// データ格納テーブルの存在確認
	bool tableIsFound();

	void createTable()   throw(DBFailureException);

	/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

	std::vector<WorkLogData> loadWorkLogByIndex(int startIndex, int endIndex);
public:
	std::vector<WorkLogData> getWorkLog();
	void writeWorkLog(WorkLogData& values) throw(DBFailureException);
};
