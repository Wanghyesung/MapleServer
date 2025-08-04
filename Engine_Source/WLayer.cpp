#include "WLayer.h"
#include "..\Engine\WEventManager.h"
#include "WScene.h"
namespace W
{
	Layer::Layer()
	{
		for (UINT i = LAYER_STARAT_IDX; i < CHAR_MAX; ++i)
			m_queueObjectID.push(i);
	}
	Layer::~Layer()
	{
		auto iter = m_hashGameObject.begin();
		for (iter; iter != m_hashGameObject.end(); ++iter)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
	void Layer::Initialize()
	{

	}
	void Layer::Update()
	{
		//RLock lock(m_lock);

		auto iter = m_hashGameObject.begin();
		for (iter; iter != m_hashGameObject.end(); ++iter)
		{
			GameObject* gameObj = iter->second;

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->Update();
		}
	}
	void Layer::LateUpdate()
	{
		//RLock lock(m_lock);

		auto iter = m_hashGameObject.begin();
		for (iter; iter != m_hashGameObject.end(); ++iter)
		{
			GameObject* gameObj = iter->second;

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->LateUpdate();
		}
	}

	void Layer::UpdatePacket()
	{
		auto iter = m_hashGameObject.begin();
		for (iter; iter != m_hashGameObject.end(); ++iter)
		{
			GameObject* gameObj = iter->second;

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->UpdatePacket();
		}
	}

	void Layer::DestroyAll(Scene* _pScene)
	{
		//WLock lock(m_lock);
		auto iter = m_hashGameObject.begin();
		for (iter; iter != m_hashGameObject.end(); ++iter)
		{
			GameObject* gameObj = iter->second;

			EventManager::DeleteObject(gameObj);
		}
	}

	GameObject* Layer::FindObject(UINT _iObjectID)
	{
		auto iter = m_hashGameObject.find(_iObjectID);
		if (iter == m_hashGameObject.end())
			return nullptr;

		return iter->second;
	}

	void Layer::AddGameObject(GameObject* _pGameObj)
	{
		if (_pGameObj->GetObjectID() == 0)
		{
			UINT iCurID = GetCurObjectID();
			_pGameObj->SetObjectID(iCurID);
			m_hashGameObject.insert(std::make_pair(iCurID, _pGameObj));
		}
		else
			m_hashGameObject.insert(std::make_pair(_pGameObj->GetObjectID(), _pGameObj));
	}

	void Layer::EraseOnHash(GameObject* _pGameObject)
	{
		UINT iGameObjectID = _pGameObject->GetObjectID();

		auto iter = m_hashGameObject.find(iGameObjectID);
		GameObject* pGameObject = iter->second;

		if (pGameObject != nullptr)
			m_hashGameObject.erase(iGameObjectID);

		//고정ID는 지우면 안됨
		if (iGameObjectID < LAYER_STARAT_IDX)
			return;

		PushObjectID(iGameObjectID);
		_pGameObject->SetObjectID(0);
	}

	UINT Layer::GetCurObjectID()
	{
		if (!m_queueObjectID.empty())
		{
			UINT iObjectID = m_queueObjectID.front();
			m_queueObjectID.pop();
			return iObjectID;
		}

		return CHAR_MAX;
	}

	void Layer::PushObjectID(UINT _iID)
	{
		if (_iID <= LAYER_STARAT_IDX)
			return;

		m_queueObjectID.push(_iID);
	}

	const unordered_map<UINT, GameObject*>& Layer::GetGameObjects()
	{
		//RLock lock(m_lock);

		return m_hashGameObject;
	}

	std::unordered_map<UINT, GameObject*> Layer::GetCopyGameObjects()
	{
		//RLock lock(m_lock);

		return m_hashGameObject;
	}

}