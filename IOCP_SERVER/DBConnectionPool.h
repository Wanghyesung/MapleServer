#pragma once
#include "DBConnection.h"

class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();

	bool Initialize();
	//bool InsertData(const wstring& _strTableName, const wstring& _strValue, int _iSize, void* _ptr);
  


	bool Connection(int _iConnectionCount, const WCHAR* _strConnection);
	void Clear();

	DBConnection* Pop();
	void Push(DBConnection* _pConnection);

private:
	RWLock m_lock;
	SQLHENV m_tEnviroment = SQL_NULL_HANDLE;
	vector<DBConnection*> m_vecConnection;

};

