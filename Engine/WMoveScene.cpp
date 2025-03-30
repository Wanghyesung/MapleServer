#include "WMoveScene.h"

#include "WResources.h"
#include "WGameObject.h"
#include "WTransform.h"

#include "WCameraScript.h"
#include "WGround.h"
#include "WInput.h"
#include "WSceneManger.h"

namespace W
{
	MoveScene::MoveScene()
	{
		
		SetMapSize(-3.f, 1.f, 3.f, -1.f);
	}
	MoveScene::~MoveScene()
	{

	}
	void MoveScene::Initialize()
	{
		CreateBackground();

	}
	void MoveScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManger::LoadScene(L"Move2");
		}

	}
	void MoveScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	
	void MoveScene::OnEnter()
	{
	}
	void MoveScene::OnExit()
	{
	}
	void MoveScene::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 1.f);
		//2 : 1
		pBackGround->GetComponent<Transform>()->SetScale(2.f * 10.f, 1.f * 10.f, 1.f);
	}
}