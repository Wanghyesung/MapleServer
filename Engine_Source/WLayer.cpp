#include "WLayer.h"
#include "..\Engine\WEventManager.h"
#include "WScene.h"
namespace W
{
	Layer::Layer():
		m_iObjectID(LAYER_STARAT_IDX)
	{
		
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
		auto iter = m_hashGameObject.begin();
		for (iter; iter != m_hashGameObject.end(); ++iter)
		{
			GameObject* gameObj = iter->second;

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->LateUpdate();
		}
	}



	void Layer::DestroyAll(Scene* _pScene)
	{
		auto iter = m_hashGameObject.begin();
		for (iter; iter != m_hashGameObject.end(); ++iter)
		{
			GameObject* gameObj = iter->second;

			EventManager::DeleteObject(gameObj);
		}
	}

	void Layer::AddGameObject(GameObject* _pGameObj)
	{
		if (_pGameObj->GetObjectID() == 0)
		{
			_pGameObj->SetObjectID(m_iObjectID);
			m_hashGameObject.insert(std::make_pair(m_iObjectID, _pGameObj));
			
			++m_iObjectID;
		}
		else
		{
			m_hashGameObject.insert(std::make_pair(_pGameObj->GetObjectID(), _pGameObj));
		}
	}

	void Layer::EraseOnVector(GameObject* _pGameObject)
	{
		UINT iGameObjectID = _pGameObject->GetObjectID();
		auto iter = m_hashGameObject.find(iGameObjectID);
		GameObject* pGameObject = iter->second;

		if (pGameObject != nullptr)
			m_hashGameObject.erase(iGameObjectID);
	}

}