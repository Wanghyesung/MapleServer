#include "WTempleScene.h"
#include "WResources.h"
#include "WGameObject.h"

#include "WTransform.h"
#include "WInput.h"
#include "WSceneManger.h"

#include "WCameraScript.h"
#include "WNPC.h"

namespace W
{
	TempleScene::TempleScene()
	{
		
		SetMapSize(-3.f, 0.f, 3.f, 0.f);
	}
	TempleScene::~TempleScene()
	{

	}
	void TempleScene::Initialize()
	{
		CreateBackground();

		NPC* pNPC = new NPC();
		
		AddGameObject(eLayerType::NPC, pNPC);
		
		pNPC->GetComponent<Transform>()->SetPosition(-8.f, -1.85f, -0.2f);
		pNPC->GetComponent<Transform>()->SetScale(0.7f * 1.1f, 1.f * 1.1f, 0.f);
		//0.7 : 1

	}
	void TempleScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManger::LoadScene(L"Valley2");
		}

	}
	void TempleScene::LateUpdate()
	{
		Scene::LateUpdate();
	}


	void TempleScene::OnEnter()
	{

	}
	void TempleScene::OnExit()
	{

	}
	void TempleScene::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
		//2.5 : 1
		pBackGround->GetComponent<Transform>()->SetScale(2.5f * 8.f, 1.f * 8.f, 1.f);
	}
}