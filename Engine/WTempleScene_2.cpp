#include "WTempleScene_2.h"
#include "WResources.h"
#include "WGameObject.h"
#include "WTransform.h"
#include "WCameraScript.h"
#include "WGround.h"
#include "WInput.h"
#include "WTexture.h"
#include "WSceneManger.h"
#include "WLadder.h"




namespace W
{
	TempleScene_2::TempleScene_2()
	{
		

		SetMapSize(-5.f,4.f,5.f,-4.f);
	}
	TempleScene_2::~TempleScene_2()
	{
	}
	void TempleScene_2::Initialize()
	{
		CreateBackground();
		setobject();

	}
	void TempleScene_2::Update()
	{
		Scene::Update();


		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManger::LoadScene(L"TempleBoss");
		}
	}
	void TempleScene_2::LateUpdate()
	{
		Scene::LateUpdate();
	}
	
	void TempleScene_2::OnEnter()
	{
	}
	void TempleScene_2::OnExit()
	{
	}
	void TempleScene_2::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
		//3 : 2
		pBackGround->GetComponent<Transform>()->SetScale(3.f * 8.f, 2.f * 8.f, 0.f);
	}
	void TempleScene_2::setobject()
	{
		Ladder* pLadder0 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder0);
		
		pLadder0->GetComponent<Transform>()->SetPosition(-7.f,-0.02f,-1.f);
		pLadder0->GetComponent<Transform>()->SetScale(1.f * 0.5f, 5.f * 0.4f, 0.f);

		Ladder* pLadder1 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder1);
		
		pLadder1->GetComponent<Transform>()->SetPosition(7.f, -0.02f, -1.f);
		pLadder1->GetComponent<Transform>()->SetScale(1.f * 0.5f, 5.f * 0.4f, 0.f);


		Ladder* pLadder2 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder2);
		
		pLadder2->GetComponent<Transform>()->SetPosition(-6.f, -4.0f, -1.f);
		pLadder2->GetComponent<Transform>()->SetScale(1.f * 0.5f, 5.f * 0.4f, 0.f);

		Ladder* pLadder3 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder3);
		
		pLadder3->GetComponent<Transform>()->SetPosition(6.f, -3.9f, -1.f);
		pLadder3->GetComponent<Transform>()->SetScale(1.f * 0.5f, 5.f * 0.4f, 0.f);
	}
}