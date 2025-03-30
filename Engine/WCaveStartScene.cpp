#include "WCaveStartScene.h"

#include "WGameObject.h"
#include "WTransform.h"
#include "WInput.h"
#include "WSceneManger.h"
#include "WCameraScript.h"
#include "WResources.h"
#include "WNPC.h"

namespace W
{
	CaveStartScene::CaveStartScene()
	{

		SetMapSize(-0.5f, 0.f, 0.5f, 0.f);

	}
	CaveStartScene::~CaveStartScene()
	{
	}
	void CaveStartScene::Initialize()
	{
		CreateBackground();
		//16 : 1
		//object camera

	}
	void CaveStartScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManger::LoadScene(L"Start2");
		}
	}
	void CaveStartScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	
	void CaveStartScene::OnEnter()
	{

	}
	void CaveStartScene::OnExit()
	{

	}
	void CaveStartScene::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 1.f);
		//1.867 : 1
		pBackGround->GetComponent<Transform>()->SetScale(1.867f * 8.f, 1.f * 8.f, 1.f);
	}
}