#pragma once
#include "../../Database/WorkLogDBHelper.h"
#include "../../Database/DBFailureException.h"
#include <string>

class ActionControl {
private:
	//std::string
	WorkLogDBHelper m_dbHelper;
	
	/* --==--==--==--==-- */
public:
	void doViewWorkLogList();
	// 再編集モードの場合true、DBのログidを引数に指定
	void doWriteWorkLog(bool reeditFlag=false, int id=-1);
	/* --==--==--==--==-- */
};
