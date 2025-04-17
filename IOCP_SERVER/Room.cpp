#include "pch.h"
#include "Room.h"
#include "ClientSession.h"
Room GRoom;

Room::Room() :
	m_iMaxCount(5),
	m_iCurCount(0)
{
	m_vecUserID.resize(6, false);
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

UINT Room::Enter(const string& _strName, shared_ptr<Session> _pSession)
{
	if (Check(_strName) == false)
		return -1;

	WLock WriteLock(m_lock);

	UINT iUserID = GetUserID();
	if (iUserID == -1)
		return -1;

	static_pointer_cast<ClientSession>(_pSession)->SetPersonID(iUserID);

	m_hashPerson.insert(make_pair(_strName, _pSession));
	m_hashPersonID.insert(make_pair(iUserID, _pSession));

	return iUserID;
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

void Room::Unicast(shared_ptr<SendBuffer> _pBuffer, vector<UINT> _vecTarget)
{
	WLock lock(m_lock);
	for (int i = 0; i < _vecTarget.size(); ++i)
	{
		m_hashPersonID[_vecTarget[i]]->Send(_pBuffer);
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
	for (int i = 1; i <= m_iMaxCount; ++i)
	{
		if (m_vecUserID[i] == false)
		{
			m_vecUserID[i] = true;
			return i;
		}
	}
	return -1;
}
