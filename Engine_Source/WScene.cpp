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
		for (UINT i = 0; i < (UINT)eLayerType::End; ++i)
		{
			m_vecLayer.push_back(new Layer());
		}
	}
	Scene::~Scene()
	{
		for (UINT i = 0; i < (UINT)eLayerType::End; ++i)
		{
			delete m_vecLayer[i];
			m_vecLayer[i] = nullptr;
		}
	}
	void Scene::Initialize()
	{
	}
	void Scene::Update()
	{
		for (Layer* layer : m_vecLayer)
		{
			layer->Update();
		}
	}
	void Scene::LateUpdate()
	{
		for (Layer* layer : m_vecLayer)
		{
			layer->LateUpdate();
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
		m_vecLayer[(UINT)_eType]->AddGameObject(_pGameObj);
		
		_pGameObj->SetLayerType(_eType);
		_pGameObj->SetSceneName(GetName());
	}

	
}