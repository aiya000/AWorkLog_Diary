#pragma once
#include "../../Database/WorkLogDBHelper.h"

/*
 * 例外catchクラス、
 * このクラスから例外がthrowされることはない。
 * (起きた全て例外はこのクラスで処理される。)
 */

class ActionControl {
private:
	WorkLogDBHelper m_dbHelper;
	
	/* --==--==--==--==-- */
private:
	bool confirm();

	/* --==--==--==--==-- */
public:
	void doViewWorkLogList();
	void doViewWorkLogDetail(int id);
	// 再編集モードの場合true、DBのログidを引数に指定
	void doWriteWorkLog(bool reeditFlag=false, int id=-1);
	void doRemoveWorkLog(int id);
	/* --==--==--==--==-- */
};
