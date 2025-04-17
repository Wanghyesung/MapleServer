#include "WTempleBossScene.h"
#include "WResources.h"
#include "WGameObject.h"

#include "WTransform.h"
#include "WInput.h"
#include "WSceneManger.h"

#include "WGround.h"
#include "WSolomon.h"
#include "WCollisionManager.h"
#include "WRex.h"
#include "WMunin.h"
#include "WAriel.h"
#include "WPinkBean.h"
#include "WWhiggin.h"
#include "WThreadPool.h"
#include "WMonsterManager.h"
namespace W
{
	TempleBossScene::TempleBossScene()
	{
		SetMapSize(-0.5f, 0.f, 0.5f, 0.f);
		SetMapPossibleSize(-7.5f, 7.5f);

		
	}
	TempleBossScene::~TempleBossScene()
	{

	}
	void TempleBossScene::Initialize()
	{
		CreateBackground();
		create_effect();
		create_monster();

		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Ground, true);
		CollisionManager::SetLayer(this, eLayerType::ItemObject, eLayerType::Ground, true);
		CollisionManager::SetLayer(this, eLayerType::ItemObject, eLayerType::Player, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Ladder, true);
		CollisionManager::SetLayer(this, eLayerType::Monster, eLayerType::AttackObject, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::MonsterAttack, true);
	}
	void TempleBossScene::Update()
	{
		Scene::Update();
	}
	void TempleBossScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void TempleBossScene::OnEnter()
	{
		Scene::OnEnter();
		ThreadPool::Joinable();
	}
	void TempleBossScene::OnExit()
	{
		Scene::OnExit();
	}
	void TempleBossScene::CreateBackground()
	{
	

		//GameObject* pBackGround = new GameObject();
		//pBackGround->SetSceneName(GetName());

		//AddGameObject(eLayerType::Background, pBackGround);
		//
		//pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
		////2 : 1
		//pBackGround->GetComponent<Transform>()->SetScale(2.f * 8.f, 1.f * 8.f, 1.f);


		Ground* pGround = new Ground(true);
		pGround->SetSceneName(GetName());
		AddGameObject(eLayerType::Ground, pGround);
		pGround->GetComponent<Transform>()->SetPosition(0.f, -2.75f, -0.1f);
		pGround->GetComponent<Transform>()->SetScale(2.7f * 7.f, 1.f * 0.3f, 0.f);
	}


	void TempleBossScene::create_monster()
	{
		PinkBean* pPinkBean = new PinkBean();
		pPinkBean->SetSceneName(GetName());
		pPinkBean->Initialize();
		AddGameObject(eLayerType::Monster, pPinkBean);
	}

	void TempleBossScene::create_effect()
	{
		
		Effect* pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"PinkBean_attack0");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(97.f, 107.f), 7, 1, Vector2(120.f, 120.f), Vector2(0.f, 0.2f), 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"PinkBean_attack1");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(124.f, 112.f), 5, 1, Vector2(120.f, 120.f), Vector2(0.f, 0.2f), 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"PinkBean_attack2");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(112.f, 110.f), 6, 1, Vector2(120.f, 120.f), Vector2(0.f, 0.2f), 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"PinkBean_attack3");
		pEffect->CreateAnimation( Vector2(0.f, 0.f), Vector2(88.f, 87.f), 4, 1, Vector2(120.f, 120.f), Vector2(0.f, 0.2f), 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"MiniBean_attack0");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(97.f, 107.f), 7, 1, Vector2(120.f, 120.f), Vector2(0.f, 0.2f), 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"Solomon_attack0");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(131.f, 128.f), 7, 1, Vector2(120.f, 120.f), Vector2(0.f,0.2f), 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"Solomon_attack2");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(217.f, 163.f), 8, 1, Vector2(200.f, 200.f), Vector2::Zero, 0.1f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"Rex_attack0");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(129.f, 128.f), 7, 1, Vector2(120.f, 120.f), Vector2(0.f, 0.2f), 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"Rex_attack2");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(219.f, 163.f), 8, 1, Vector2(200.f, 200.f), Vector2::Zero, 0.1f);
	
		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"Munin_attack0");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(117.f, 156.f), 5, 1, Vector2(150.f, 150.f), Vector2(0.f, 0.2f), 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"Munin_attack1");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(109.f, 104.f), 8, 1, Vector2(200.f, 200.f), Vector2::Zero, 0.1f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"Whiggin_attack1");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(109.f, 104.f), 8, 1, Vector2(200.f, 200.f), Vector2::Zero, 0.1f);

		pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->SetName(L"Whiggin_attack0");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(117.f, 156.f), 5, 1, Vector2(150.f, 150.f), Vector2(0.f, 0.2f), 0.2f);

		
	
	}
}