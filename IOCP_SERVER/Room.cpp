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
	m_hashSendMask[iUserID] = true;

	m_iCurCount.fetch_add(1);

	return iUserID;
}


void Room::Exit(const string& _strName, UINT _ID)
{
	WLock lock(m_lock);

	if (m_hashPerson.find(_strName) == m_hashPerson.end())
		return;

	m_hashPerson.erase(_strName);
	m_hashPersonID.erase(_ID);

	m_vecUserID[_ID] = false;
	m_hashSendMask[_ID] = true;

	//m_hashPersonID.erase()
	m_iCurCount.fetch_sub(1);
}

void Room::Broadcast(shared_ptr<SendBuffer> _pBuffer)
{
	std::vector<std::shared_ptr<Session>> vecTarget;
	{
		RLock lock(m_lock);
		vecTarget.reserve(m_hashPersonID.size());
		for (auto& iter : m_hashPersonID)
		{
			if (m_hashSendMask[iter.first])
				vecTarget.push_back(iter.second);
		}
	}

	for (auto& iter : vecTarget)
	{
		iter->Send(_pBuffer);
	}
}

void Room::BroadcastExcept(shared_ptr<SendBuffer> _pBuffer, shared_ptr<Session> _pExceptSession)
{
	std::vector<std::shared_ptr<Session>> vecTarget;
	{
		RLock lock(m_lock);
		vecTarget.reserve(m_hashPersonID.size());
		for (auto& iter : m_hashPersonID)
		{
			if (iter.second == _pExceptSession)
				continue;

			if (m_hashSendMask[iter.first])
				vecTarget.push_back(iter.second);
		}
	}

	for (auto& iter : vecTarget)
	{
		iter->Send(_pBuffer);
	}
}

void Room::Unicast(shared_ptr<SendBuffer> _pBuffer, const vector<UINT>& _vecTarget)
{
	std::vector<UINT> vecTarget;
	{
		RLock lock(m_lock);
		vecTarget.reserve(_vecTarget.size());
		for (auto iter : _vecTarget)
		{
			if (m_hashSendMask[iter])
				vecTarget.push_back(iter);
		}
	}

	for (auto& iter : vecTarget)
	{
		m_hashPersonID[iter]->Send(_pBuffer);
	}
}

void Room::Unicast(shared_ptr<SendBuffer> _pBuffer, const unordered_set<UINT>& _setTarget)
{
	std::vector<UINT> vecTarget;
	{
		RLock lock(m_lock);
		vecTarget.reserve(_setTarget.size());
		for (auto iter : _setTarget)
		{
			if (m_hashSendMask[iter])
				vecTarget.push_back(iter);
		}
	}

	for (auto& iter : vecTarget)
	{
		m_hashPersonID[iter]->Send(_pBuffer);
	}
}

void Room::UnicastExcept(shared_ptr<SendBuffer> _pBuffer, const vector<UINT>& _setTarget, UINT _iExceptID)
{
	std::vector<UINT> vecTarget;
	{
		RLock lock(m_lock);
		vecTarget.reserve(_setTarget.size());
		for (auto iter : _setTarget)
		{
			if (m_hashSendMask[iter] && iter !=_iExceptID)
				vecTarget.push_back(iter);
		}
	}
	for (auto& iter : vecTarget)
	{
		m_hashPersonID[iter]->Send(_pBuffer);
	}
}

vector<UINT> Room::GetPersons()
{
	std::vector<UINT> vecID = {};
	{
		RLock ReadLock(m_lock);
		vecID.reserve(m_hashPerson.size());
		for (auto iter : m_hashPerson)
		{
			UINT ID = static_pointer_cast<ClientSession>(iter.second)->GetPersonID();
			vecID.push_back(ID);
		}
	}

	return vecID;
}

shared_ptr<Session> Room::GetPersonByID(UINT _ID)
{
	if(m_hashPersonID.find(_ID) == m_hashPersonID.end())
		return nullptr;
	
	return m_hashPersonID[_ID];
}

UINT Room::GetUserID()
{
	for (UINT i = 1; i <= m_iMaxCount; ++i)
	{
		if (m_vecUserID[i] == false)
		{
			m_vecUserID[i] = true;
			return i;
		}
	}
	return -1;
}
