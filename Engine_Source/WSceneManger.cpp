#include "WSceneManger.h"

#include "WRigidbody.h"

#include "..\Engine\WPlayer.h"
#include "..\Engine\WBattleManager.h"

#include "..\Engine\WPlayerAttackObject.h"
#include "..\Engine\WObjectPoolManager.h"
#include "..\Engine\WMonsterManager.h"
namespace W
{
	UINT SceneManger::SCENE_IDX = 0;
	std::unordered_map<std::wstring, Scene*> SceneManger::m_hashScene = {};
	std::vector<std::wstring> SceneManger::m_vecPlayerScene = {};

	void SceneManger::Initialize()
	{
		

	}
	void SceneManger::Update()
	{
		for (const std::wstring& strScene : m_vecPlayerScene)
		{
			m_hashScene.find(strScene)->second->Update();
		}
	}
	void SceneManger::LateUpdate()
	{
		for (const std::wstring& strScene : m_vecPlayerScene)
		{
			m_hashScene.find(strScene)->second->LateUpdate();
		}
	}

	void SceneManger::Destroy()
	{
		//m_pActiveScene->Destroy();
	}

	void SceneManger::Release()
	{
		for (auto &iter : m_hashScene)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		ObjectPoolManager::Release();
		BattleManager::Release();
	}

	void SceneManger::Erase(GameObject* _pGameObject)
	{
		Scene* pScene = FindScene(_pGameObject->GetSceneName());
		pScene->EraseObject(_pGameObject->GetLayerType(), _pGameObject);
	}

	Scene* SceneManger::FindScene(const std::wstring& _strSceneName)
	{
		const auto& iter = m_hashScene.find(_strSceneName);
		if(iter == m_hashScene.end())
			return nullptr;

		return iter->second;
	}

	Scene* SceneManger::GetActiveScene(GameObject* _pGameObj)
	{
		const std::wstring& strSceneName = _pGameObj->GetSceneName();
		Scene* pScene = FindScene(strSceneName);

		if(!pScene)
			return nullptr;

		return pScene;
	}

	std::vector<Scene*> SceneManger::GetPlayerScene()
	{
		std::vector<Scene*> vecPlayerScene;
		for (int i = 0; i < m_vecPlayerScene.size(); ++i)
		{
			vecPlayerScene.push_back(m_hashScene.find(m_vecPlayerScene[i])->second);
		}
		return vecPlayerScene;
	}

	void SceneManger::AddGameObject(const std::wstring& _strSceneName, eLayerType _eType, GameObject* _pGameObj)
	{
		Scene* pScene = FindScene(_strSceneName);
		//다음 프레임에 넣기, 후처리에 넣기
		pScene->AddGameObject(_eType, _pGameObj);
	}

	GameObject* SceneManger::FindPlayer(UINT _iPlayerID)
	{
		for (Scene* pScene : GetPlayerScene())
		{
			const std::vector<GameObject*>& vecObjs = pScene->GetLayer(eLayerType::Player).GetGameObjects();
			for (GameObject* pObj : vecObjs)
			{
				if (((Player*)pObj)->GetPlayerID() == _iPlayerID)
					return pObj;
			}
		}
		return nullptr;
	}

	GameObject* SceneManger::FindPlayer(const std::wstring& _strSceneName)
	{
		Scene* pScene = m_hashScene[_strSceneName];
		const std::vector<GameObject*> vecPlayer = pScene->GetLayer(eLayerType::Player).GetGameObjects();

		if(vecPlayer.empty())
			return nullptr;
		
		return vecPlayer[0];
	}

	const std::vector<GameObject*>& SceneManger::GetPlayers(const std::wstring& _strSceneName)
	{
		return m_hashScene[_strSceneName]->GetLayer(eLayerType::Player).GetGameObjects();
	}

	void SceneManger::SwapObject(Scene* _pPrevScene, Scene* _pNextScene, GameObject* _pGameObject)
	{
		eLayerType eType = _pGameObject->GetLayerType();
		
		_pPrevScene->EraseObject(eType, _pGameObject);
		_pNextScene->AddGameObject(eType, _pGameObject);
	}


	void SceneManger::SwapPlayer(Player* _pPlayer, Scene* _pPrevScene, Scene* _pNextScene)
	{
		
		Vector3 vPos = _pPlayer->GetComponent<Transform>()->GetPosition();
		vPos.x = 0.f; vPos.y = 0.f;
		_pPlayer->GetComponent<Transform>()->SetPosition(vPos);
		_pPlayer->GetComponent<Rigidbody>()->SetGround(false);

		SwapObject(_pPrevScene, _pNextScene, _pPlayer);
	}


	void SceneManger::PushObjectPool(Scene* _pPrevScene)
	{
		std::vector<GameObject*> m_vecObj =
			_pPrevScene->GetLayer(eLayerType::AttackObject).GetGameObjects();

		for (int i = 0; i < m_vecObj.size(); ++i)
			dynamic_cast<PlayerAttackObject*>(m_vecObj[i])->Off();
	}

	void SceneManger::AddPlayerScene(const std::wstring& _strScene)
	{
		m_vecPlayerScene.push_back(_strScene);
	}

}
