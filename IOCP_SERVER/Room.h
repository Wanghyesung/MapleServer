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
	UINT Enter(const string& _strName, shared_ptr<Session> _pSession);

	void Exit(const string& _strName);

	void Broadcast(shared_ptr<SendBuffer> _pBuffer);
	void BroadcastExcept(shared_ptr<SendBuffer> _pBuffer, shared_ptr<Session> _pExceptSession);
	void Unicast(shared_ptr<SendBuffer> _pBuffer, const vector<UINT>& _vecTarget);

	vector<UINT> GetPersons();
	shared_ptr<Session> GetPersonByID(UINT _ID);
private:
	UINT GetUserID();

private:
	RWLock m_lock;


	unordered_map<string, shared_ptr<Session>> m_hashPerson;
	unordered_map<UINT, shared_ptr<Session>> m_hashPersonID;

	atomic<UINT> m_iMaxCount;
	atomic<UINT> m_iCurCount;
	vector<bool> m_vecUserID;

};


extern Room GRoom;