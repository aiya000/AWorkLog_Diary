#pragma once
#include "../../Database/WorkLogDBHelper.h"
#include "../../Database/WorkLogData.h"
#include <tr1/array>
#include <stdexcept>
#include "../../Lib/SystemUtils.hpp"

/*
 * 例外catchクラス、
 * このクラスから例外がthrowされることはない。
 * (起きた全て例外はこのクラスで処理される。)
 */

class ActionControl {
private:
	WorkLogDBHelper m_dbHelper;
	
	// listp,listnコマンドに使用(current)
	struct {
		int start, end;
	} m_range;
	/* --==--==--==--==-- */
public:
	ActionControl();

	/* --==--==--==--==-- */
private:
	bool confirm();
	void viewDetails(WorkLogData workLog, bool viewComment=false);
	void viewColumn(WorkLogData workLog);

	/* --==--==--==--==-- */
public:
	void incrementRange();  //rel->doViewWorkLogList()
	void decrementRange();  //rel->doViewWorkLogList()

	/* --==--==--==--==-- */
	void doViewWorkLogList();
	void doFindFunction(std::string keyword);
	void doSearchFunction(std::string regex);
	void doViewWorkLogDetail(int id);
	// 再編集モードの場合true、DBのログidを引数に指定
	void doEditWorkLog(bool reeditFlag=false, int id=-1);
	void doRemoveWorkLog(int id);
	void doBackupWorkLogFile();
	void doRestoreWorkLogFile();
	void doLsBackupWorkLogFile();

	/* --==--==--==--==-- */

private:
	// <- doEditWorkLog & editDetails
	struct OperationInterruptedException : public std::domain_error {
		OperationInterruptedException(std::string str);
	};
	// doEditWorkLog->
	std::tr1::array<std::string,3>
		editDetails(
			const std::string& funcPath,
			const std::string& tarPath,
			const std::string& comPath)
		throw(alib::SystemCommandCallException, OperationInterruptedException);
	// doEditWorkLog->
	std::tr1::array<std::string,3>
		editDetailsUseStdin(
			const std::string& comPath,
			bool reeditFlag,
			int  id)
		throw(alib::SystemCommandCallException, OperationInterruptedException);

	/* --==--==--==--==-- */
};
