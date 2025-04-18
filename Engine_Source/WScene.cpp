#include "WScene.h"
#include "WThreadPool.h"
#include "..\Engine\WMonsterHP.h"
#include "..\Engine\WMonsterManager.h"
namespace W
{
	UINT Scene::SCENECOUNT = 0;
	Scene::Scene():
		m_iSceneIdx(SCENECOUNT++)
	{
		m_vecLayer.resize((UINT)eLayerType::End);
	}
	Scene::~Scene()
	{
	}
	void Scene::Initialize()
	{
	}
	void Scene::Update()
	{
		for (Layer& layer : m_vecLayer)
		{
			layer.Update();
		}
	}
	void Scene::LateUpdate()
	{
		for (Layer& layer : m_vecLayer)
		{
			layer.LateUpdate();
		}
	}


	void Scene::OnEnter()
	{
		
	}
	void Scene::OnExit()
	{
		
	}
	void Scene::AddGameObject(eLayerType _eType, GameObject* _pGameObj)
	{
		m_vecLayer[(UINT)_eType].AddGameObject(_pGameObj);
		
		_pGameObj->SetLayerType(_eType);
		_pGameObj->SetSceneName(GetName());
	}

	
}