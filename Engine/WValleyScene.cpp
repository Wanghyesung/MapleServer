#include "WValleyScene.h"
#include "WResources.h"

#include "WGameObject.h"

#include "WTransform.h"
#include "WInput.h"
#include "WSceneManger.h"
#include "WCameraScript.h"
#include "WLadder.h"
#include "WNPC.h"
#include "WCage.h"

#include "WPlayer.h"
#include "WObject.h"

#include "WGround.h"
#include "WCollisionManager.h"
#include "WTexture.h"
#include "WSkelegon.h"
#include "WPortal.h"
#include "WPortalScript.h"

namespace W
{
	ValleyScene::ValleyScene()
	{

		SetMapSize(0.f, 7.f, 0.f, -7.f);
		SetMapPossibleSize(-4.37f, 4.37f);
	}
	ValleyScene::~ValleyScene()
	{

	}
	void ValleyScene::Initialize()
	{
		CreateBackground();
		setobject();
		setmonster();

		//Player* pPlayer = new Player();
		//AddGameObject(eLayerType::Player, pPlayer);
		//pPlayer->Initialize();


		NPC* pNPC = new NPC();
		AddGameObject(eLayerType::NPC, pNPC);
		
		pNPC->GetComponent<Transform>()->SetPosition(2.5f, 7.5f, -0.2f);
		pNPC->GetComponent<Transform>()->SetScale(0.761f, 1.f, 0.f);
		
		//{
		//	GameObject* pCamera = new GameObject();
		//	pCamera->SetName(L"ObjCam");
		//	AddGameObject(eLayerType::Camera, pCamera);
		//	pCamera->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, -10.f));
		//	Camera* pCameraComp = pCamera->AddComponent<Camera>();
		//	pCameraComp->TurnLayerMask(eLayerType::UI, false);
		//	CameraScript* pCameraScript = pCamera->AddComponent<CameraScript>();
		//	renderer::MainCamera = pCameraComp;
		//	pCameraScript->SetPlayer(pPlayer);
		//}

		////ui camera
		//{
		//	GameObject* pUICamera = new GameObject();
		//	pUICamera->SetName(L"UICam");
		//	AddGameObject(eLayerType::Camera, pUICamera);
		//	pUICamera->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, -10.f));
		//	Camera* pCameraComp = pUICamera->AddComponent<Camera>();
		//	pCameraComp->TurnUILayerMask();//UI만 그리게
		//	renderer::UICamera = pCameraComp;
		//}
	}
	void ValleyScene::Update()
	{
		Scene::Update();
	}
	void ValleyScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void ValleyScene::OnEnter()
	{
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Ground, true);
		CollisionManager::SetLayer(this, eLayerType::ItemObject, eLayerType::Ground, true);
		CollisionManager::SetLayer(this, eLayerType::ItemObject, eLayerType::Player, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Ladder, true);
		CollisionManager::SetLayer(this, eLayerType::Monster, eLayerType::AttackObject, true);
		CollisionManager::SetLayer(this, eLayerType::Monster, eLayerType::Player, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::MonsterAttack, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Portal, true);

	}
	void ValleyScene::OnExit()
	{
		CollisionManager::Clear();
	}
	void ValleyScene::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		

		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.2f);
		pBackGround->GetComponent<Transform>()->SetScale(25 * 0.35f,  25.f * 1.f, 1.f);
	}


	void ValleyScene::setobject()
	{
		Cage* pCage = new Cage();
		AddGameObject(eLayerType::Box, pCage);
		
		pCage->GetComponent<Transform>()->SetPosition(2.5f, 7.57f, -0.3f);
		pCage->GetComponent<Transform>()->SetScale(0.641f * 2.2f, 1.f * 2.2f, 0.f);
		//0.641 : 1

		Ladder* pLadder0 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder0);

		pLadder0->GetComponent<Transform>()->SetPosition(0.f, -4.4f, -0.1f);
		pLadder0->GetComponent<Transform>()->SetScale(1.f * 0.6f, 5.5f * 0.6f, 0.f);
		pLadder0->Initialize();
		pLadder0->GetComponent<Collider2D>()->SetCenter(Vector2(0.f, -0.1f));
	

		Ladder* pLadder1 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder1);
		
		pLadder1->GetComponent<Transform>()->SetPosition(2.f, 4.2f, -0.1f);
		pLadder1->GetComponent<Transform>()->SetScale(1.f * 0.6f, 5.6f * 0.6f, 0.f);
		pLadder1->Initialize();

		Ladder* pLadder2 = new Ladder();
		AddGameObject(eLayerType::Ladder, pLadder2);
	
		pLadder2->GetComponent<Transform>()->SetPosition(-1.f, -0.7f, -0.1f);
		pLadder2->GetComponent<Transform>()->SetScale(1.f * 0.6f, 4.5f * 0.6f, 0.f);
		pLadder2->Initialize();

		Ground* pGround1 = new Ground();
		AddGameObject(eLayerType::Ground, pGround1);
		pGround1->GetComponent<Transform>()->SetPosition(0.f, 0.48f, -0.1f);
		pGround1->GetComponent<Transform>()->SetScale(2.7f * 5.f, 1.f * 0.3f, 0.f);


		Ground* pGround2 = new Ground();
		AddGameObject(eLayerType::Ground, pGround2);
		pGround2->GetComponent<Transform>()->SetPosition(-0.6f, -3.f, -0.1f);
		pGround2->GetComponent<Transform>()->SetScale(2.7f * 1.5f, 1.f * 0.3f, 0.f);

		Ground* pGround3 = new Ground(true);
		AddGameObject(eLayerType::Ground, pGround3);
		pGround3->GetComponent<Transform>()->SetPosition(0.f, -7.6f, -0.1f);
		pGround3->GetComponent<Transform>()->SetScale(2.7f * 5.f, 1.f * 0.3f, 0.f);

		Portal* pPortal = new Portal();
		AddGameObject(eLayerType::Portal, pPortal);
		//pPortal->GetScript<PortalScript>()->SetNextScene(L"Helisium");
		pPortal->GetScript<PortalScript>()->SetNextScene(L"DemianScene");
		//pPortal->GetScript<PortalScript>()->SetNextScene(L"Cave");
		pPortal->GetComponent<Transform>()->SetPosition(3.6f, -6.8f, -0.1f);

		Portal* pPortal2 = new Portal();
		AddGameObject(eLayerType::Portal, pPortal2);
		pPortal2->GetScript<PortalScript>()->SetNextScene(L"TempleBoss");
		pPortal2->GetComponent<Transform>()->SetPosition(-3.6f, -6.8f, -0.1f);


	}
	void ValleyScene::setmonster()
	{
		Skelegon* pSkelegon = object::Instantiate<Skelegon>(Vector3(3.f, 1.2f, -1.5f), eLayerType::Monster);
		pSkelegon->Initialize();
		

	}
}
