#pragma once
class CPerson;
class Session;

class Room
{
public:
	Room();
	~Room();

public:
	bool Check(const string& _strName);
	bool Enter(const string& _strName, shared_ptr<Session> _pSession);
	void Exit(const string& _strName);
	void Broadcast(shared_ptr<SendBuffer> _pBuffer);
	void BroadcastExcept(shared_ptr<SendBuffer> _pBuffer, shared_ptr<Session> _pExceptSession);

	vector<UINT> GetPersons();

private:
	RWLock m_lock;

	unordered_map<string, shared_ptr<Session>> m_hashPerson;

	atomic<INT> m_iMaxCount;
	atomic<INT> m_iCurCount;
};


extern Room GRoom;