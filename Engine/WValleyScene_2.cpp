#include "WValleyScene_2.h"
#include "WResources.h"
#include "WGameObject.h"

#include "WTransform.h"
#include "WInput.h"
#include "WSceneManger.h"

#include "WCameraScript.h"
#include "WLadder.h"
#include "WNPC.h"

namespace W
{
	ValleyScene_2::ValleyScene_2()
	{
		
		SetMapSize(-1.f, 0.f, 1.f, 0.f);
	}
	ValleyScene_2::~ValleyScene_2()
	{

	}
	void ValleyScene_2::Initialize()
	{
		CreateBackground();
		setobject();

		NPC* pNPC = new NPC();
		AddGameObject(eLayerType::NPC, pNPC);
	
		pNPC->GetComponent<Transform>()->SetPosition(6.f, 2.48f, -1.f);
		pNPC->GetComponent<Transform>()->SetScale(1.147f * 1.f, 1.f * 1.f, 0.f);
		//1.147 : 1

	}
	void ValleyScene_2::Update()
	{
		Scene::Update();
		
		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManger::LoadScene(L"Temple2");
		}
	}
	void ValleyScene_2::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void ValleyScene_2::OnEnter()
	{

	}
	void ValleyScene_2::OnExit()
	{

	}
	void ValleyScene_2::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		AddGameObject(eLayerType::Background, pBackGround);
	
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
		//2 : 1
		pBackGround->GetComponent<Transform>()->SetScale(2 * 8.f, 1.f * 8.f, 1.f);
	}
	void ValleyScene_2::setobject()
	{
		Ladder* pLadder0 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder0);
		
		pLadder0->GetComponent<Transform>()->SetPosition(-2.f,-0.6f,-1.f);
		pLadder0->GetComponent<Transform>()->SetScale(1.f * 0.5f, 4.f * 0.7f, 0.f);

	}
}