#pragma once
#include <sql.h>
#include <sqlext.h>

class DBConnection
{
	enum
	{
		WVARCHAR_MAX = 4000,
		BINARY_MAX = 8000
	};

public:
	bool Connect(SQLHENV _tHenv, const WCHAR* _strConnection);
	void Clear();

	bool Prepare(const WCHAR* _strQuery);
	bool Execute(const WCHAR* _strQuery);
	bool ExecutePrepared();

	bool Fetch(); //쿼리 실행
	int GetRowCount();//데이터가 몇개있는지
	void UnBind();

public:
	bool			BindParam(int _iParamIndex, bool* _value, SQLLEN* _iIndex);
	bool			BindParam(int _iParamIndex, float* _value, SQLLEN* _iIndex);
	bool			BindParam(int _iParamIndex, double* _value, SQLLEN* _iIndex);
	bool			BindParam(int _iParamIndex, char* _value, SQLLEN* _iIndex);
	bool			BindParam(int _iParamIndex, short* _value, SQLLEN* _iIndex);
	bool			BindParam(int _iParamIndex, int* _value, SQLLEN* _iIndex);
	bool			BindParam(int _iParamIndex, long long* _value, SQLLEN* _iIndex);
	bool			BindParam(int _iParamIndex, TIMESTAMP_STRUCT* _value, SQLLEN* _iIndex);
	bool			BindParam(int _iParamIndex, const WCHAR* _str, SQLLEN* _iIndex);
	bool			BindParam(int _iParamIndex, const BYTE* _bin, int iSize, SQLLEN* _iIndex);

	bool			BindCol(int _icolumnIndex, bool* _value, SQLLEN* _iIndex);
	bool			BindCol(int _icolumnIndex, float* _value, SQLLEN* _iIndex);
	bool			BindCol(int _icolumnIndex, double* _value, SQLLEN* _iIndex);
	bool			BindCol(int _icolumnIndex, char* _value, SQLLEN* _iIndex);
	bool			BindCol(int _icolumnIndex, short* _value, SQLLEN* _iIndex);
	bool			BindCol(int _icolumnIndex, int* _value, SQLLEN* _iIndex);
	bool			BindCol(int _icolumnIndex, long long* _value, SQLLEN* _iIndex);
	bool			BindCol(int _icolumnIndex, TIMESTAMP_STRUCT* _value, SQLLEN* _iIndex);
	bool			BindCol(int _icolumnIndex, WCHAR* _str, int iSize, SQLLEN* _iIndex);
	bool			BindCol(int _icolumnIndex, BYTE* _bin, int iSize, SQLLEN* _iIndex);

private:
	//SQL문의 ?에 값 집어넣기
	bool BindParam(SQLUSMALLINT _sParamIdx, SQLUSMALLINT _sType, SQLSMALLINT _sSQLType,
		SQLULEN _lLen, SQLPOINTER _ptr, SQLLEN* _index);
	//결과의 컬럼 값 꺼내오기
	bool BindCol(SQLUSMALLINT _sColumnIdx, SQLSMALLINT _sType, SQLULEN _lLen, SQLPOINTER _value, SQLLEN* _index);

	void HandleError(SQLRETURN _sRet);

private:
	SQLHDBC m_tConnection = SQL_NULL_HANDLE; //SQL 커넥션 담당 핸들
	SQLHSTMT m_tStatement = SQL_NULL_HANDLE; //해당 상태를 통해 DB에 데이터 연결
};

