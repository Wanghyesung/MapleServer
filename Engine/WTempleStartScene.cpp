#include "WTempleStartScene.h"
#include "WGameObject.h"

#include "WTransform.h"
#include "WInput.h"
#include "WSceneManger.h"
#include "WCameraScript.h"
#include "WResources.h"
#include "WNPC.h"

namespace W
{
	TempleStartScene::TempleStartScene()
	{
		
	}
	TempleStartScene::~TempleStartScene()
	{

	}
	void TempleStartScene::Initialize()
	{
		CreateBackground();

		NPC* pNPC = new NPC();
		AddGameObject(eLayerType::NPC, pNPC);
	
		pNPC->GetComponent<Transform>()->SetPosition(-4.5f, -2.3f, -1.f);
		pNPC->GetComponent<Transform>()->SetScale(0.547f * 1.4f, 1.f * 1.4f, 0.f);
		pNPC->GetComponent<Transform>()->SetRotation(0.f,0.f,0.f);
		//0.547 : 1

	}
	void TempleStartScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManger::LoadScene(L"Leafre");
		}
	}
	void TempleStartScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	
	void TempleStartScene::OnEnter()
	{

	}
	void TempleStartScene::OnExit()
	{

	}
	void TempleStartScene::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 1.f);
		//16 : 9
		pBackGround->GetComponent<Transform>()->SetScale(16.f * 1.f, 9.f * 1.f, 1.f);
	}
}