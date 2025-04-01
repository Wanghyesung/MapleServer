#include "WCaveScene.h"
#include "WResources.h"
#include "WGameObject.h"
#include "WTransform.h"
#include "WGround.h"
#include "WInput.h"
#include "WSceneManger.h"
#include "WGround.h"
#include "WCollisionManager.h"
#include "WStone.h"
#include "WHorntail.h"
#include "WStoneScript.h"
#include "WLadder.h"
#include "WThreadPool.h"
#include "WMonsterManager.h"

namespace W
{
	CaveScene::CaveScene()
	{
		SetMapSize(0.f, 1.f, 0.f, -1.f);
		SetMapPossibleSize(-7.f, 7.f);
	}
	CaveScene::~CaveScene()
	{
		
	}
	void CaveScene::Initialize()
	{
		create_object();
		create_effect();
		create_monster();

		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Ground, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Box, true);
		CollisionManager::SetLayer(this, eLayerType::AttackObject, eLayerType::Box, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Ladder, true);
		CollisionManager::SetLayer(this, eLayerType::Monster, eLayerType::AttackObject, true);
		CollisionManager::SetLayer(this, eLayerType::Monster, eLayerType::Player, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::MonsterAttack, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Portal, true);
	}
	void CaveScene::Update()
	{
		Scene::Update();
	}
	void CaveScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	
	void CaveScene::OnEnter()
	{
		Scene::OnEnter();
		
		ThreadPool::Joinable();
	}
	void CaveScene::OnExit()
	{
		Scene::OnExit();//리소스 해제

		
		ThreadPool::Joinable();
	}
	void CaveScene::CreateBackground()
	{
		GameObject* pBackGround = new GameObject();
		pBackGround->SetSceneName(GetName());
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 1.f);
		//14::10
		pBackGround->GetComponent<Transform>()->SetScale(14.f * 1.f, 10.f * 1.f, 1.f);
		
	}
	
	void CaveScene::create_object()
	{	
		Ground* pGround = new Ground(true);
		pGround->SetSceneName(GetName());
		AddGameObject(eLayerType::Ground, pGround);
		pGround->GetComponent<Transform>()->SetPosition(0.f, -3.8f, -0.1f);
		pGround->GetComponent<Transform>()->SetScale(2.7f * 5.f, 1.f * 0.3f, 0.f);

		pGround = new Ground();
		pGround->SetSceneName(GetName());
		AddGameObject(eLayerType::Ground, pGround);
		pGround->GetComponent<Transform>()->SetPosition(2.71f, -1.7f, -0.1f);
		pGround->GetComponent<Transform>()->SetScale(1.f * 0.6f, 1.f * 0.2f, 0.f);

		pGround = new Ground();
		pGround->SetSceneName(GetName());
		AddGameObject(eLayerType::Ground, pGround);
		pGround->GetComponent<Transform>()->SetPosition(2.8f, -0.7f, -0.1f);
		pGround->GetComponent<Transform>()->SetScale(1.f * 0.6f, 1.f * 0.2f, 0.f);
		//
		pGround = new Ground();
		pGround->SetSceneName(GetName());
		AddGameObject(eLayerType::Ground, pGround);
		pGround->GetComponent<Transform>()->SetPosition(4.3f, -1.18f, -0.1f);
		pGround->GetComponent<Transform>()->SetScale(1.f * 2.f, 1.f * 0.2f, 0.f);

		Ladder* pLadder2 = new Ladder();
		pGround->SetSceneName(GetName());
		AddGameObject(eLayerType::Ladder, pLadder2);
		
		pLadder2->GetComponent<Transform>()->SetPosition(3.5f, -2.4f, -0.1f);
		pLadder2->GetComponent<Transform>()->SetScale(1.f * 0.6f, 4.5f * 0.6f, 0.f);
		pLadder2->Initialize();
		pLadder2->GetComponent<Collider2D>()->SetSize(Vector2(0.3f, 0.9f));
		pLadder2->GetComponent<Collider2D>()->SetCenter(Vector2(0.f, 0.11f));
	


	}
	void CaveScene::create_effect()
	{	
		const std::wstring& strSceneName = GetName();
		Effect* pEffect = new Effect();
		pEffect->SetSceneName(strSceneName);
		pEffect->SetName(L"icebreath");
		pEffect->CreateAnimation( Vector2(0.f, 0.f), Vector2(110.f, 113.f), 2, 1, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(strSceneName);
		pEffect->SetName(L"ice");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(118.f, 87.f), 2, 1, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

	
		pEffect = new Effect();
		pEffect->SetSceneName(strSceneName);
		pEffect->SetName(L"firebreath");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(99.f, 107.f), 2, 1, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

		pEffect = new Effect();
		pEffect->SetName(L"fire");
		pEffect->SetSceneName(strSceneName);
		pEffect->CreateAnimation( Vector2(0.f, 0.f), Vector2(92.f, 91.f), 4, 1, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(strSceneName);
		pEffect->SetName(L"thunderbreath");
		pEffect->CreateAnimation( Vector2(0.f, 0.f), Vector2(72.f, 82.f), 4, 1, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(strSceneName);
		pEffect->SetName(L"thunder");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(78.f, 75.f), 5, 1, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(strSceneName);
		pEffect->SetName(L"legattack1");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(126.f, 107.f), 5, 1, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

		pEffect = new Effect();
		pEffect->SetSceneName(strSceneName);
		pEffect->SetName(L"legattack2");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(197, 119.f), 6, 1, Vector2(100.f, 100.f), Vector2(0.f, 0.3f), 0.1f);
	
		pEffect = new Effect();
		pEffect->SetSceneName(strSceneName);
		pEffect->SetName(L"tailattack");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(132.f, 120.f), 5, 1, Vector2(100.f, 100.f), Vector2(0.f, 0.3f), 0.2f);
	}

	void CaveScene::create_monster()
	{
		Stone* pStone = new Stone();
		pStone->SetSceneName(GetName());
		AddGameObject(eLayerType::Box, pStone);
		pStone->GetComponent<Transform>()->SetPosition(4.45f, 0.f, -0.1f);

		Horntail* pHorntail = new Horntail();
		pHorntail->SetSceneName(GetName());
		pHorntail->Initialize();
		AddGameObject(eLayerType::Monster, pHorntail);
		pHorntail->GetComponent<Transform>()->SetPosition(-1.2f, 0.73f, -1.5f);

		std::function<void()> start = std::bind(&Horntail::Start, pHorntail);
		pStone->GetScript<StoneScript>()->SetFunction(start);
	}

}