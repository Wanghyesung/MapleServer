#include "pch.h"
#include "Room.h"
#include "ClientSession.h"
Room GRoom;

Room::Room() :
	m_iMaxCount(5)
{
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

	if (m_hashPerson.size() > m_iMaxCount)
		return false;

	return true;
}

bool Room::Enter(const string& _strName, shared_ptr<Session> _pSession)
{
	WLock WriteLock(m_lock);

	m_hashPerson.insert(make_pair(_strName, _pSession));

	m_iCurCount.fetch_add(1);
	return true;
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
	RLock ReadLock(m_lock);

	std::vector<UINT> vecID = {};
	for (auto iter : m_hashPerson)
	{
		UINT ID = static_pointer_cast<ClientSession>(iter.second)->GetPersonID();
		vecID.push_back(ID);
	}

	return vecID;
}
