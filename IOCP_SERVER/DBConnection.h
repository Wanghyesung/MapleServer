#pragma once
#include <sql.h>
#include <sqlext.h>

class DBConnection
{
public:
	bool Connect(SQLHENV _tHenv, const WCHAR* _strConnection);
	void Clear();

	bool Prepare(const WCHAR* _strQuery);
	bool Execute(const WCHAR* _strQuery);
	bool ExecutePrepared();

	bool Fetch(); //결과 받아오기
	int GetRowCount();//데이터가 몇개있는지
	void UnBind();

public:
	//쿼리 실행
	bool BindParam(SQLUSMALLINT _sParamIdx, SQLUSMALLINT _sType, SQLSMALLINT _sSQLType,
		SQLULEN _lLen, SQLPOINTER _ptr, SQLLEN* _index);

	//쿼리 받아올 때
	bool BindCol(SQLUSMALLINT _sColumnIdx, SQLSMALLINT _sType, SQLULEN _lLen, SQLPOINTER _value, SQLLEN* _index);
	void HandleError(SQLRETURN _sRet);

private:
	SQLHDBC m_tConnection = SQL_NULL_HANDLE; //SQL 커넥션 담당 핸들
	SQLHSTMT m_tStatement = SQL_NULL_HANDLE; //해당 상태를 통해 DB에 데이터 연결
};

