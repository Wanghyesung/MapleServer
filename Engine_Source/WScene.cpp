#include "WScene.h"
#include "WThreadPool.h"
#include "..\Engine\WMonsterHP.h"
#include "..\Engine\WMonsterManager.h"
namespace W
{
	Scene::Scene()
	{
		m_vecLayer.resize((UINT)W::enums::eLayerType::End);
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

	void Scene::Destroy()
	{
		for (UINT i = 0; i < m_vecLayer.size(); ++i)
		{
			if (i == (UINT)eLayerType::UI)
				continue;

			m_vecLayer[i].Destory();
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
	}

	
}