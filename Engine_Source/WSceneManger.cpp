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
	std::unordered_map<std::wstring, std::vector<UINT>> SceneManger::m_hashPlayerScene = {};

	void SceneManger::Initialize()
	{
		

	}
	void SceneManger::Update()
	{
		auto iter = m_hashPlayerScene.begin();
		for (iter; iter != m_hashPlayerScene.end(); ++iter)
		{
			if (iter->second.size() > 0)
			{
				m_hashScene.find(iter->first)->second->Update();
			}
		}
	}

	void SceneManger::LateUpdate()
	{
		auto iter = m_hashPlayerScene.begin();
		for (iter; iter != m_hashPlayerScene.end(); ++iter)
		{
			if (iter->second.size() > 0)
			{
				m_hashScene.find(iter->first)->second->LateUpdate();
			}
		}

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
		std::vector<Scene*> vecPlayerScene = {};

		auto iter = m_hashPlayerScene.begin();
		for (iter; iter != m_hashPlayerScene.end(); ++iter)
		{
			if (iter->second.size() > 0)
			{
				vecPlayerScene.push_back(m_hashScene.find(iter->first)->second);
			}
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
			const std::unordered_map<UINT,GameObject*>& hashObjs
				= pScene->GetLayer(eLayerType::Player)->GetGameObjects();

			auto iter = hashObjs.find(_iPlayerID);
			if (iter != hashObjs.end())
				return iter->second;

			//auto iter = hashObjs.begin();
			//for (iter; iter != hashObjs.end(); ++iter)
			//{
			//	GameObject* pObj = iter->second;
			//
			//	if (((Player*)pObj)->GetPlayerID() == _iPlayerID)
			//		return pObj;
			//}
		}
		return nullptr;
	}

	GameObject* SceneManger::FindPlayer(const std::wstring& _strSceneName)
	{
		Scene* pScene = m_hashScene[_strSceneName];
		std::unordered_map<UINT, GameObject*> hashPlayer = 
			pScene->GetLayer(eLayerType::Player)->GetGameObjects();

		if(hashPlayer.empty())
			return nullptr;
		
		srand(time(nullptr));

		int iPlayerCount = hashPlayer.size();
		int iNumber = rand() % iPlayerCount;

		auto iter = hashPlayer.begin();
		for (int i = 0; i < iNumber; ++i)
			++iter;

		return iter->second;
	}

	std::vector<GameObject*> SceneManger::GetPlayers(const std::wstring& _strSceneName)
	{
		std::vector<GameObject*> vecPlayer;

		const std::unordered_map<UINT, GameObject*>& hashObjs
			= m_hashScene[_strSceneName]->GetLayer(eLayerType::Player)->GetGameObjects();

		auto iter = hashObjs.begin();
		for (iter; iter != hashObjs.end(); ++iter)
		{
			vecPlayer.push_back(iter->second);
		}
		return vecPlayer;
	}

	std::vector<UINT> SceneManger::GetPlayerIDs(const std::wstring& _strSceneName)
	{
		return m_hashPlayerScene[_strSceneName];
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

		UINT iPlayerID = _pPlayer->GetPlayerID();
		std::wstring strPrevSceneName = _pPrevScene->GetName();
		std::wstring strNextSceneName = _pNextScene->GetName();

		// 이전 씬에서 해당 플레이어 ID 제거
		auto& iterPrev = m_hashPlayerScene[strPrevSceneName];
		iterPrev.erase(
			std::remove(iterPrev.begin(), iterPrev.end(), iPlayerID),
			iterPrev.end()
		);

		// 다음 씬에 플레이어 ID 추가
		m_hashPlayerScene[strNextSceneName].push_back(iPlayerID);
	}


	void SceneManger::PushObjectPool(Scene* _pPrevScene)
	{
		const std::unordered_map<UINT, GameObject*>& hashObjs
			= _pPrevScene->GetLayer(eLayerType::AttackObject)->GetGameObjects();

		auto iter = hashObjs.begin();
		for (iter; iter != hashObjs.end(); ++iter)
			dynamic_cast<PlayerAttackObject*>(iter->second)->Off();
	}

	void SceneManger::AddPlayerScene(Player* pPlayer, const std::wstring& _strScene)
	{
		AddGameObject(_strScene, eLayerType::Player, pPlayer);

		m_hashPlayerScene[_strScene].push_back(pPlayer->GetPlayerID());
	}

}
