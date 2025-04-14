#include "pch.h"
#include "Room.h"
#include "ClientSession.h"
Room GRoom;

Room::Room() :
	m_iMaxCount(5),
	m_iCurCount(0)
{
	m_vecUserID.resize(5, false);
}

Room::~Room()
{
}

bool Room::Check(const string& _strName)
{
	//읽기 전용
	RLock ReadLock(m_lock);

	if (m_hashPerson.find(_strName) != m_hashPerson.end())
		return false;

	if (m_hashPerson.size() >= m_iMaxCount)
		return false;

	return true;
}

bool Room::Enter(const string& _strName, shared_ptr<Session> _pSession)
{
	if (Check(_strName) == false)
		return false;

	WLock WriteLock(m_EnterLock);
	static_pointer_cast<ClientSession>(_pSession)->SetPersonID(GetUserID());
	m_queueEnter.push(make_pair(_strName, _pSession));

	return true;
}

//Engine쪽에서 접근할 예정
vector<UINT> Room::CheckEnterQueue()
{
	vector<UINT> vecUserID = {};
	{
		WLock WriteLock(m_EnterLock);

		while (!m_queueEnter.empty())
		{
			auto& pSession = m_queueEnter.front();
			m_queueEnter.pop();

			vecUserID.push_back(static_pointer_cast<ClientSession>(pSession.second)->GetPersonID());

			//Enter자원을 들고 main 자원을 요청 -> main은 하나의 락으로만 동작 데드락 X
			WLock lock(m_lock); 
			m_hashPerson.insert(make_pair(pSession.first, pSession.second));
			m_iCurCount.fetch_add(1);
		}
	}
	
	return vecUserID;
}

void Room::Exit(const string& _strName)
{
	WLock lock(m_lock);

	if (m_hashPerson.find(_strName) == m_hashPerson.end())
		return;

	m_hashPerson.erase(_strName);

	m_iCurCount.fetch_sub(1);
}

void Room::Broadcast(shared_ptr<SendBuffer> _pBuffer)
{
	WLock lock(m_lock);
	for (auto& iter : m_hashPerson)
	{
		iter.second->Send(_pBuffer);
	}
}

void Room::BroadcastExcept(shared_ptr<SendBuffer> _pBuffer, shared_ptr<Session> _pExceptSession)
{
	WLock lock(m_lock);
	for (auto& iter : m_hashPerson)
	{
		if (iter.second == _pExceptSession)
			continue;

		iter.second->Send(_pBuffer);
	}
}

vector<UINT> Room::GetPersons()
{
	std::vector<UINT> vecID = {};

	{
		RLock ReadLock(m_lock);
		for (auto iter : m_hashPerson)
		{
			UINT ID = static_pointer_cast<ClientSession>(iter.second)->GetPersonID();
			vecID.push_back(ID);
		}
	}

	return vecID;
}

UINT Room::GetUserID()
{
	for (int i = 0; i < m_iMaxCount; ++i)
	{
		if (m_vecUserID[i] == false)
		{
			m_vecUserID[i] = true;
			return 1;
		}
	}
	return -1;
}
