#include "WHelisiumBoss.h"
#include "WResources.h"
#include "WCollisionManager.h"
#include "WGround.h"
#include "WMegnus.h"
#include "WThreadPool.h"
#include "WMonsterManager.h"

namespace W
{
	HelisiumBoss::HelisiumBoss()
	{
		SetMapSize(-7.5f, -1.f, 7.5f, -1.f);
		SetMapPossibleSize(-14.f, 14.f);

	}
	HelisiumBoss::~HelisiumBoss()
	{

	}
	void HelisiumBoss::Initialize()
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
		CollisionManager::SetLayer(this, eLayerType::Ground, eLayerType::MonsterAttack, true);
	}
	void HelisiumBoss::Update()
	{
		Scene::Update();
	}
	void HelisiumBoss::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void HelisiumBoss::OnEnterPlayer(UINT _iPlayerID)
	{
	}

	void HelisiumBoss::OnExitPlayer(UINT _iPlayerID)
	{
	}
	
	void HelisiumBoss::OnEnter()
	{
		Scene::OnEnter();
		ThreadPool::Joinable();
	}
	void HelisiumBoss::OnExit()
	{
		Scene::OnExit();
	}
	void HelisiumBoss::CreateBackground()
	{
		Ground* pGround = new Ground(true);
		pGround->SetSceneID(GetSceneID());
		pGround->GetComponent<Transform>()->SetPosition(0.f, -2.95f, -0.1f);
		pGround->GetComponent<Transform>()->SetScale(4.3f * 7.f, 1.f * 0.3f, 0.f);
		AddGameObject(eLayerType::Ground, pGround);
	}
	
	void HelisiumBoss::create_monster()
	{
		Megnus* pMegnus = new Megnus();
		pMegnus->SetSceneID(GetSceneID());

		pMegnus->Initialize();
		AddGameObject(eLayerType::Monster, pMegnus);
	}

	void HelisiumBoss::create_effect()
	{
		
		Effect* pEffect = new Effect();
		pEffect->SetSceneID(GetSceneID());
		pEffect->SetName(L"Megnus_attack0_hit");
		pEffect->CreateAnimation( Vector2(0.f, 0.f), Vector2(320.f, 244.f), 5, 1, Vector2(320.f, 320.f), Vector2(0.f, 0.f), 0.1f);

		pEffect = new Effect();
		pEffect->SetSceneID(GetSceneID());
		pEffect->SetName(L"Megnus_attack1_hit");
		pEffect->CreateAnimation( Vector2(0.f, 0.f), Vector2(256.f, 196.f), 5, 1, Vector2(250.f, 250.f), Vector2(0.f, 0.f), 0.1f);

		pEffect = new Effect();
		pEffect->SetSceneID(GetSceneID());
		pEffect->SetName(L"Megnus_attack2_hit");
		pEffect->CreateAnimation( Vector2(0.f, 0.f), Vector2(236.f, 229.f), 7, 1, Vector2(240.f, 240.f), Vector2(0.f, 0.f), 0.1f);

		pEffect = new Effect();
		pEffect->SetSceneID(GetSceneID());
		pEffect->SetName(L"Megnus_attack3_hit");
		pEffect->CreateAnimation( Vector2(0.f, 0.f), Vector2(228.f, 231.f), 7, 1, Vector2(230.f, 230.f), Vector2(0.f, 0.f), 0.1f);

		pEffect = new Effect();
		pEffect->SetSceneID(GetSceneID());
		pEffect->SetName(L"Megnus_attack4_hit");
		pEffect->CreateAnimation( Vector2(0.f, 0.f), Vector2(165.f, 168.f), 7, 1, Vector2(170.f, 170.f), Vector2(0.f, 0.f), 0.1f);

	}
}