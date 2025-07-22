#pragma once
#include "DBConnection.h"

template<int C>
struct FullBits { enum { value = (1 << (C - 1)) | FullBits<C - 1>::value }; };

template<>
struct FullBits<1> { enum { value = 1 }; };

template<>
struct FullBits<0> { enum { value = 0 }; };

template<int ParamCount, int ColumnCount>
class DBBind
{
public:
	DBBind(DBConnection& dbConnection, const WCHAR* query)
		: m_dbConnection(dbConnection), m_strQuery(query)
	{
		::memset(m_paramIndex, 0, sizeof(m_paramIndex));
		::memset(m_columnIndex, 0, sizeof(m_columnIndex));
		m_paramFlag = 0;
		m_columnFlag = 0;
		dbConnection.UnBind();
	}

	bool Validate()
	{
		return m_paramFlag == FullBits<ParamCount>::value && m_columnFlag == FullBits<ColumnCount>::value;
	}

	bool Execute()
	{
		if (Validate() == false)
			return false;

		return m_dbConnection.Execute(m_strQuery);
	}

	bool Fetch()
	{
		return m_dbConnection.Fetch();
	}

public:
	template<typename T>
	void BindParam(int idx, T& value)
	{
		m_dbConnection.BindParam(idx + 1, &value, &m_paramIndex[idx]);
		m_paramFlag |= (1LL << idx);
	}

	void BindParam(int idx, const WCHAR* value)
	{
		m_dbConnection.BindParam(idx + 1, value, &m_paramIndex[idx]);
		m_paramFlag |= (1LL << idx);
	}

	void BindParam(int idx, const CHAR* value)
	{
		m_dbConnection.BindParam(idx + 1, value, &m_paramIndex[idx]);
		m_paramFlag |= (1LL << idx);
	}

	template<typename T, int N>
	void BindParam(int idx, T(&value)[N])
	{
		m_dbConnection.BindParam(idx + 1, (const BYTE*)value, size32(T) * N, &m_paramIndex[idx]);
		m_paramFlag |= (1LL << idx);
	}

	template<typename T>
	void BindParam(int idx, T* value, int N)
	{
		m_dbConnection.BindParam(idx + 1, (const BYTE*)value, size32(T) * N, &m_paramIndex[idx]);
		m_paramFlag |= (1LL << idx);
	}

	template<typename T>
	void BindCol(int idx, T& value)
	{
		m_dbConnection.BindCol(idx + 1, &value, &m_columnIndex[idx]);
		m_columnFlag |= (1LL << idx);
	}

	template<int N>
	void BindCol(int idx, WCHAR(&value)[N])
	{
		m_dbConnection.BindCol(idx + 1, value, N - 1, &m_columnIndex[idx]);
		m_columnFlag |= (1LL << idx);
	}

	void BindCol(int idx, WCHAR* value, int len)
	{
		m_dbConnection.BindCol(idx + 1, value, len - 1, &m_columnIndex[idx]);
		m_columnFlag |= (1LL << idx);
	}

	template<typename T, int N>
	void BindCol(int idx, T(&value)[N])
	{
		m_dbConnection.BindCol(idx + 1, value, size32(T) * N, &m_columnIndex[idx]);
		m_columnFlag |= (1LL << idx);
	}

protected:
	DBConnection& m_dbConnection;
	const WCHAR* m_strQuery;
	SQLLEN			m_paramIndex[ParamCount > 0 ? ParamCount : 1];
	SQLLEN			m_columnIndex[ColumnCount > 0 ? ColumnCount : 1];
	UINT64			m_paramFlag;
	UINT64			m_columnFlag;
};

