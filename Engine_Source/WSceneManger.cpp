#include "WSceneManger.h"

#include "WRigidbody.h"
//#include "WPlayScene.h"
//#include "WLeafreScene.h"
//#include "WValleyScene.h"
//#include "WValleyScene_2.h"
//#include "WCaveScene.h"
//#include "WTempleScene.h"
//#include "WTempleScene_2.h"
//#include "WTempleBossScene.h"
#include "..\Engine\WCameraScript.h"

#include "..\Engine\WBattleManager.h"

#include "..\Engine\WPlayerAttackObject.h"
#include "..\Engine\WObjectPoolManager.h"
#include "..\Engine\WMonsterManager.h"
namespace W
{
	std::vector<std::wstring> SceneManger::m_vecPlayerScene = {};

	Scene* SceneManger::m_pActiveScene = nullptr;
	std::map<std::wstring, Scene*> SceneManger::m_mapScene = {};
	void SceneManger::Initialize()
	{
		

	}
	void SceneManger::Update()
	{
		for (const std::wstring& strScene : m_vecPlayerScene)
		{
			m_mapScene.find(strScene)->second->Update();
		}
	}
	void SceneManger::LateUpdate()
	{
		for (const std::wstring& strScene : m_vecPlayerScene)
		{
			m_mapScene.find(strScene)->second->LateUpdate();
		}
	}

	void SceneManger::Destroy()
	{
		m_pActiveScene->Destroy();
	}

	void SceneManger::Release()
	{
		for (auto &iter : m_mapScene)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		ObjectPoolManager::Release();
		BattleManager::Release();
	}

	void SceneManger::Erase(GameObject* _pGameObject)
	{
		m_pActiveScene->EraseObject(_pGameObject->GetLayerType(),_pGameObject);
	}

	std::vector<Scene*> SceneManger::GetPlayerScene()
	{
		std::vector<Scene*> vecPlayerScene;
		for (int i = 0; i < m_vecPlayerScene.size(); ++i)
		{
			vecPlayerScene.push_back(m_mapScene.find(m_vecPlayerScene[i])->second);
		}
		return vecPlayerScene;
	}

	Scene* SceneManger::LoadScene(std::wstring _strName)
	{
		std::map<std::wstring, Scene*>::iterator iter =
			m_mapScene.find(_strName);

		if (iter == m_mapScene.end())
			return nullptr;


		PushObjectPool(m_pActiveScene);//현재 씬 공격 오브젝트 회수
		
		SwapPlayer(m_pActiveScene, iter->second);

		m_pActiveScene->OnExit();
		m_pActiveScene = iter->second;
		m_pActiveScene->OnEnter();

		//SwapCamera();
		return iter->second;
	}

	void SceneManger::AddGameObject(eLayerType _eType, GameObject* _pGameObj)
	{
		//다음 프레임에 넣기, 후처리에 넣기
		m_pActiveScene->AddGameObject(_eType, _pGameObj);
	}

	GameObject* SceneManger::FindPlayer()
	{
		std::vector<GameObject*> vecObjs = 
			m_pActiveScene->GetLayer(eLayerType::Player).GetGameObjects();

		return vecObjs[0];
	}

	void SceneManger::SwapObject(Scene* _pPrevScene, Scene* _pNextScene, GameObject* _pGameObject)
	{
		eLayerType eType = _pGameObject->GetLayerType();
		
		_pPrevScene->EraseObject(eType, _pGameObject);
		_pNextScene->AddGameObject(eType, _pGameObject);
	}


	void SceneManger::SwapPlayer(Scene* _pPrevScene, Scene* _pNextScene)
	{
		GameObject* pPlayer = FindPlayer();
		Vector3 vPos = pPlayer->GetComponent<Transform>()->GetPosition();
		vPos.x = 0.f; vPos.y = 0.f;
		pPlayer->GetComponent<Transform>()->SetPosition(vPos);
		pPlayer->GetComponent<Rigidbody>()->SetGround(false);

		SwapObject(_pPrevScene, _pNextScene, pPlayer);
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
