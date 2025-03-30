#include "WMoveScene_2.h"
#include "WResources.h"
#include "WGameObject.h"

#include "WTransform.h"
#include "WInput.h"
#include "WSceneManger.h"

#include "WCameraScript.h"

namespace W
{
	MoveScene_2::MoveScene_2()
	{

		SetMapSize(-3.f, 1.f, 3.f, -1.f);
	}

	MoveScene_2::~MoveScene_2()
	{

	}
	void MoveScene_2::Initialize()
	{
		CreateBackground();

	}
	void MoveScene_2::Update()
	{
		Scene::Update();

		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManger::LoadScene(L"Start1");
		}
	}
	void MoveScene_2::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void MoveScene_2::OnEnter()
	{

	}
	void MoveScene_2::OnExit()
	{

	}
	void MoveScene_2::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 1.f);
		//2 : 1
		pBackGround->GetComponent<Transform>()->SetScale(2.f * 10.f, 1.f * 10.f, 1.f);
	}
}