#include "WLeafreScene.h"

#include "WResources.h"
#include "WGameObject.h"
#include "WTransform.h"

#include "WCameraScript.h"
#include "WGround.h"
#include "WInput.h"
#include "WSceneManger.h"
#include "WLadder.h"
#include "WNPC.h"

namespace W
{
	LeafreScene::LeafreScene()
	{
		

		SetMapSize(-4.8f,2.f,4.8f,-2.5f);
	}
	LeafreScene::~LeafreScene()
	{

	}
	void LeafreScene::Initialize()
	{
		CreateBackground();
		setobject();
		
		NPC* pNPC = new NPC();
		AddGameObject(eLayerType::NPC, pNPC);
	
		pNPC->GetComponent<Transform>()->SetPosition( -0.3f, -2.7f, -0.03f);
		pNPC->GetComponent<Transform>()->SetScale(0.69 * 1.3f, 0.89 * 1.3f,0.f);

		NPC* pNPC2 = new NPC();
		AddGameObject(eLayerType::NPC, pNPC2);
		
		pNPC2->GetComponent<Transform>()->SetPosition(-7.f, -3.85f, -0.03f);
		pNPC2->GetComponent<Transform>()->SetScale(0.763 * 1.1f, 1.f * 1.1f, 0.f);
		//0.763 : 1
	
	}
	void LeafreScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManger::LoadScene(L"Valley");
		}
	}
	void LeafreScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	
	void LeafreScene::OnEnter()
	{

	}
	void LeafreScene::OnExit()
	{

	}
	void LeafreScene::CreateBackground()
	{
		//¹ÙÅÁ
		{
			GameObject* pBackGround = new GameObject();
			AddGameObject(eLayerType::Background, pBackGround);
			
			pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
			//1.829 : 1
			pBackGround->GetComponent<Transform>()->SetScale(1.829f * 13.f, 1.f * 13.f, 1.f);
		}
	}
	void LeafreScene::setobject()
	{
		Ladder* pLadder0 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder0);
		
		pLadder0->GetComponent<Transform>()->SetPosition(-4.f, -1.6f, -0.02f);
		pLadder0->GetComponent<Transform>()->SetScale(0.4f * 1.8f, 1.5f * 1.5f, 0.f);

		Ladder* pLadder1 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder1);
	
		pLadder1->GetComponent<Transform>()->SetPosition(3.6f, -1.37f, -0.02f);
		pLadder1->GetComponent<Transform>()->SetScale(0.4f * 1.8f, 1.5f * 1.5f, 0.f);
	}
}