#include "WStartScene.h"
#include "WResources.h"
#include "WCameraScript.h"
#include "WInput.h"
#include "WSceneManger.h"

namespace W
{
	StartScene::StartScene()
	{
	
	}
	StartScene::~StartScene()
	{

	}
	void StartScene::Initialize()
	{
		
		CreateBackground();
		
	}
	void StartScene::Update()
	{
		Scene::Update();
	}
	void StartScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void StartScene::Destroy()
	{
	}
	void StartScene::OnEnter()
	{
	}
	void StartScene::OnExit()
	{

	}
	void StartScene::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.2f);
		//035 : 1
		pBackGround->GetComponent<Transform>()->SetScale(4 * 3.5f, 3.f * 2.7f, 1.f);
	}
}