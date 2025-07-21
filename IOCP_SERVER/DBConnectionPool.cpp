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
	auto query = L"											    \
    DROP TABLE IF EXISTS [dbo].[Equip];						    \
    CREATE TABLE [dbo].[Equip] (								\
    [Name]      NVARCHAR(50) NOT NULL PRIMARY KEY,				\
    [EyeID]     INT         NOT NULL DEFAULT 0,					\
    [HairID]    INT         NOT NULL DEFAULT 0,					\
    [HatID]     INT         NOT NULL DEFAULT 0,					\
    [TopID]     INT         NOT NULL DEFAULT 0,					\
    [BottomID]  INT         NOT NULL DEFAULT 0,					\
    [ShoesID]   INT         NOT NULL DEFAULT 0,					\
    [WeaponID]  INT         NOT NULL DEFAULT 0					\
	);";

	DBConnection* dbConn = Pop();
	if (dbConn->Execute(query) == false)
		return false;
	Push(dbConn);
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
