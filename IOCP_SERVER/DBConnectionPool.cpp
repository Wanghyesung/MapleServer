#include "pch.h"
#include "DBConnectionPool.h"

DBConnectionPool::DBConnectionPool()
{

}

DBConnectionPool::~DBConnectionPool()
{
	Clear();
}

bool DBConnectionPool::Initialize()
{
	auto query = L"											   \
    DROP TABLE IF EXISTS [dbo].[Equip];						   \
    CREATE TABLE [dbo].[Equip]								   \
    (                                                          \
        [Name]    VARCHAR(50) NOT NULL PRIMARY KEY IDENTITY    \
        [EyeID]   INT         NOT NULL,						   \
    );";

	/*
		[HairID]  INT         NOT NULL,                 \
        [HatID]   INT         NOT NULL,                 \
        [TopID]   INT         NOT NULL,                 \
        [BottomID]INT         NOT NULL,                 \
        [ShoesID] INT         NOT NULL,                 \
        [WeaponID]INT         NOT NULL                  \
	*/
	DBConnection* dbConn = Pop();
	if (dbConn->Execute(query) == false)
		return false;
	Push(dbConn);
}

bool DBConnectionPool::InsertData(const wstring& _strTableName, const wstring& _strValue , int _iSize, void* _ptr)
{
	DBConnection* pDB = Pop();
	// 기존에 바인딩 된 정보 날림
	pDB->UnBind();


	wstring strResult = L"INSERT INTO " + _strTableName + L"([" + _strValue + L"]) VALUES(?)";
	
	// 넘길 인자 바인딩
	SQLLEN len = 0;
	// 넘길 인자 바인딩
	if (pDB->BindParam(1, SQL_C_LONG, SQL_INTEGER, _iSize, _ptr, &len) == false)
		return false;

	// SQL 실행

	if (pDB->Execute(strResult.c_str()) == false)
		return false;

	Push(pDB);
	return true;
}

//DB사용하는 스레드 수 만큼 카운트
bool DBConnectionPool::Connection(int _iConnectionCount, const WCHAR* _strConnection)
{
	WLock lock(m_lock);

	//sql환경 설정
	if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_tEnviroment) != SQL_SUCCESS)
		return false;

	if (::SQLSetEnvAttr(m_tEnviroment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
		return false;

	for (int i = 0; i < _iConnectionCount; ++i)
	{
		DBConnection* pDBConnection = new DBConnection();
		if (pDBConnection->Connect(m_tEnviroment, _strConnection) == false)
			return false;


		m_vecConnection.push_back(pDBConnection);
	}
	return true;
}

void DBConnectionPool::Clear()
{

	WLock lock(m_lock);
	if (m_tEnviroment != SQL_NULL_HANDLE)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_tEnviroment);
		m_tEnviroment = SQL_NULL_HANDLE;
	}

	for (DBConnection* pDBConnection : m_vecConnection)
		delete pDBConnection;

	m_vecConnection.clear();
}

DBConnection* DBConnectionPool::Pop()
{
	WLock lock(m_lock);
	if (m_vecConnection.empty())
		return nullptr;

	DBConnection* pConnection = m_vecConnection.back();
	m_vecConnection.pop_back();

	return pConnection;
}

void DBConnectionPool::Push(DBConnection* _pConnection)
{
	WLock lock(m_lock);
	m_vecConnection.push_back(_pConnection);
}
