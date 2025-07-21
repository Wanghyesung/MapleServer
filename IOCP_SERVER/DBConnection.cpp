#include "pch.h"
#include "DBConnection.h"

bool DBConnection::Connect(SQLHENV _tHenv, const WCHAR* _strConnection)
{
    if (::SQLAllocHandle(SQL_HANDLE_DBC, _tHenv, &m_tConnection) != SQL_SUCCESS)
        return false;
    WCHAR strBuffer[MAX_PATH] = { 0 };
    ::wcscpy_s(strBuffer, _strConnection);

    WCHAR strResult[MAX_PATH] = { 0 };
    SQLSMALLINT strResultLen = 0;

    //DB연결
    SQLRETURN ret = SQLDriverConnectW(
        m_tConnection,
        NULL,
        reinterpret_cast<SQLWCHAR*>(strBuffer),
        _countof(strBuffer),
        OUT reinterpret_cast<SQLWCHAR*>(strResult),
        _countof(strResult),
        OUT & strResultLen,
        SQL_DRIVER_NOPROMPT
    );

    //sqlstatement
    if (SQLAllocHandle(SQL_HANDLE_STMT, m_tConnection, &m_tStatement) != SQL_SUCCESS)
        return false;

    return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);

}

void DBConnection::Clear()
{
    if (m_tConnection != SQL_NULL_HANDLE)
    {
        SQLFreeHandle(SQL_HANDLE_DBC, m_tConnection);
        m_tConnection = SQL_NULL_HANDLE;
    }

    if (m_tStatement != SQL_NULL_HANDLE)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, m_tStatement);
        m_tStatement = SQL_NULL_HANDLE;
    }
}

bool DBConnection::Prepare(const WCHAR* _strQuery)
{
    SQLRETURN ret = ::SQLPrepareW(m_tStatement, (SQLWCHAR*)_strQuery, SQL_NTSL);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
   
    HandleError(ret);
    return false;
}

bool DBConnection::Execute(const WCHAR* _strQuery)
{
    SQLRETURN ret = ::SQLExecDirectW(m_tStatement, (SQLWCHAR*)_strQuery, SQL_NTSL);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;

    HandleError(ret);
    return false;
}

bool DBConnection::ExecutePrepared()
{
    SQLRETURN ret = ::SQLExecute(m_tStatement);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    HandleError(ret);
    return false;
}

bool DBConnection::Fetch()
{
    SQLRETURN ret = SQLFetch(m_tStatement);

    switch (ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        return true;
    case SQL_NO_DATA:
        return false;
    case SQL_ERROR:
        HandleError(ret);
        return false;
    default:
        true;
    }

    return false;
}

int DBConnection::GetRowCount()
{
    SQLLEN iCount = 0;
    SQLRETURN ret = SQLRowCount(m_tStatement, &iCount);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return static_cast<int>(iCount);

    return -1;
}

void DBConnection::UnBind()
{
    //매칭된 값 미리 없애기
    SQLFreeStmt(m_tStatement, SQL_UNBIND);
    SQLFreeStmt(m_tStatement, SQL_RESET_PARAMS);
    SQLFreeStmt(m_tStatement, SQL_CLOSE);
}



bool DBConnection::BindParam(SQLUSMALLINT _sParamIdx, SQLUSMALLINT _sType, SQLSMALLINT _sSQLType, SQLULEN _lLen, SQLPOINTER _ptr, SQLLEN* _index)
{
    SQLRETURN ret = SQLBindParameter(m_tStatement, _sParamIdx, SQL_PARAM_INPUT, _sType, _sSQLType, _lLen, 0, _ptr, 0, _index);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        HandleError(ret);
        return false;
    }

    return true;
}

bool DBConnection::BindCol(SQLUSMALLINT _sColumnIdx, SQLSMALLINT _sType, SQLULEN _lLen, SQLPOINTER _value, SQLLEN* _index)
{
    SQLRETURN ret = SQLBindCol(m_tStatement, _sColumnIdx, _sType, _value, _lLen, _index);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        HandleError(ret);
        return false;
    }

    return true;
}

void DBConnection::HandleError(SQLRETURN _sRet)
{
    if (_sRet == SQL_SUCCESS)
        return;

    SQLSMALLINT index = 1;
    SQLWCHAR sqlState[MAX_PATH] = { 0 };
    SQLINTEGER iNativeErr = 0;
    SQLWCHAR errMsg[MAX_PATH] = { 0 };
    SQLSMALLINT msgLen = 0;
    SQLRETURN errorRet = 0;

    while (true)
    {
        errorRet = SQLGetDiagRecW(
            SQL_HANDLE_STMT,
            m_tStatement,
            index,
            sqlState,
            OUT & iNativeErr,
            errMsg,
            _countof(errMsg),
            OUT & msgLen
        );

        if (errorRet == SQL_NO_DATA)
            break;

        if (errorRet == SQL_SUCCESS || errorRet == SQL_SUCCESS_WITH_INFO)
            break;

        //TODO : log
        ++index;
    }
}
