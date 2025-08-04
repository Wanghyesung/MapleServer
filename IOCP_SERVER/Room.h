#pragma once

class CPerson;
class Session;
#include "DBConnectionPool.h"

enum eRoomCount
{
	MAXCOUNT = 16
};
class Room
{
public:
	Room();
	~Room();

public:
	bool Check(const string& _strName);
	UINT Enter(const string& _strName, shared_ptr<Session> _pSession);

	void Exit(const string& _strName, UINT _ID);

	void Broadcast(shared_ptr<SendBuffer> _pBuffer);
	void BroadcastExcept(shared_ptr<SendBuffer> _pBuffer, shared_ptr<Session> _pExceptSession);
	void Unicast(shared_ptr<SendBuffer> _pBuffer, const vector<UINT>& _vecTarget);
	void Unicast(shared_ptr<SendBuffer> _pBuffer, const unordered_set<UINT>& _setTarget);
	void UnicastExcept(shared_ptr<SendBuffer> _pBuffer, const vector<UINT>& _setTarget, UINT _iExceptID);

	void LockSendBask(UINT _iSessionID) { m_hashSendMask[_iSessionID] = false; }
	void UnLockSendBask(UINT _iSessionID) { m_hashSendMask[_iSessionID] = true; }

	vector<UINT> GetPersons();
	shared_ptr<Session> GetPersonByID(UINT _ID);
private:
	UINT GetUserID();

private:
	RWLock m_lock;


	unordered_map<string, weak_ptr<Session>> m_hashPerson;
	unordered_map<UINT, weak_ptr<Session>> m_hashPersonID;
	std::unordered_map<UINT, bool> m_hashSendMask;

	atomic<UINT> m_iMaxCount;
	atomic<UINT> m_iCurCount;
	vector<bool> m_vecUserID;

};

extern DBConnectionPool* GDBConnectionPool;
extern Room GRoom;