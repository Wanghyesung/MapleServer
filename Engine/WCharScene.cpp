#include "WCharScene.h"
#include "WResources.h"

#include "WCameraScript.h"
#include "WInput.h"
#include "WSceneManger.h"
namespace W
{
	CharScene::CharScene()
	{
		
	}
	CharScene::~CharScene()
	{

	}
	void CharScene::Initialize()
	{
		CreateBackground();

	}
	void CharScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManger::LoadScene(L"Valley");
		}
	}
	void CharScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void CharScene::OnEnter()
	{
	}
	void CharScene::OnExit()
	{

	}
	void CharScene::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.2f);
	
		pBackGround->GetComponent<Transform>()->SetScale(4 * 3.5f, 3.f * 3.f, 1.f);
	}
}