#include "WSeleteScene.h"

#include "WResources.h"

#include "WCameraScript.h"
#include "WInput.h"
#include "WSceneManger.h"


namespace W
{
	SeleteScene::SeleteScene()
	{
		

	}
	SeleteScene::~SeleteScene()
	{

	}
	void SeleteScene::Initialize()
	{

		CreateBackground();
		
	}
	void SeleteScene::Update()
	{
		Scene::Update();
	}
	void SeleteScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void SeleteScene::Destroy()
	{
	}
	void SeleteScene::OnEnter()
	{
	}
	void SeleteScene::OnExit()
	{

	}
	void SeleteScene::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.2f);
		//035 : 1
		pBackGround->GetComponent<Transform>()->SetScale(4 * 3.5f, 3.f * 2.7f, 1.f);
	}
}