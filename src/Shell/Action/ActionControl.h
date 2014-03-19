#pragma once
#include "../../Database/WorkLogDBHelper.h"
#include "../../Database/WorkLogData.h"
#include <tr1/array>
#include "../../Lib/SystemUtils.hpp"

/*
 * 例外catchクラス、
 * このクラスから例外がthrowされることはない。
 * (起きた全て例外はこのクラスで処理される。)
 */

class ActionControl {
private:
	WorkLogDBHelper m_dbHelper;
	
	/* --==--==--==--==-- */

	/* --==--==--==--==-- */
private:
	bool confirm();
	void viewDetails(WorkLogData workLog, bool viewComment=false);

	/* --==--==--==--==-- */
public:
	void doViewWorkLogList();
	void doViewWorkLogDetail(int id);
	// 再編集モードの場合true、DBのログidを引数に指定
	void doEditWorkLog(bool reeditFlag=false, int id=-1);
	void doRemoveWorkLog(int id);

	/* --==--==--==--==-- */

private:
	// doEditWorkLog->
	std::tr1::array<std::string,3>
		editDetails(
			const std::string& funcPath,
			const std::string& tarPath,
			const std::string& comPath)
		throw(alib::SystemCommandCallException, alib::SystemInterruptedException);
	// doEditWorkLog->
	std::tr1::array<std::string,3>
		editDetailsUseStdin(
			const std::string& comPath,
			bool reeditFlag,
			int  id)
		throw(alib::SystemCommandCallException, alib::SystemInterruptedException);

	/* --==--==--==--==-- */
};
