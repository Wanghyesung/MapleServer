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
	vector<UINT> CheckEnterQueue();
	void Exit(const string& _strName);
	void Broadcast(shared_ptr<SendBuffer> _pBuffer);
	void BroadcastExcept(shared_ptr<SendBuffer> _pBuffer, shared_ptr<Session> _pExceptSession);

	vector<UINT> GetPersons();

private:
	UINT GetUserID();

private:
	RWLock m_lock;
	RWLock m_EnterLock;

	unordered_map<string, shared_ptr<Session>> m_hashPerson;
	queue<pair<string, shared_ptr<Session>>> m_queueEnter;

	atomic<UINT> m_iMaxCount;
	atomic<UINT> m_iCurCount;
	vector<bool> m_vecUserID;

};


extern Room GRoom;