#include "WValleyScene.h"
#include "WResources.h"

#include "WGameObject.h"

#include "WTransform.h"
#include "WInput.h"
#include "WSceneManger.h"
#include "WLadder.h"
#include "WCage.h"

#include "WPlayer.h"

#include "WGround.h"
#include "WCollisionManager.h"
#include "WSkelegon.h"


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
		//CreateBackground();
		setobject();
		setmonster();

		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Ground, true);
		CollisionManager::SetLayer(this, eLayerType::ItemObject, eLayerType::Ground, true);
		CollisionManager::SetLayer(this, eLayerType::ItemObject, eLayerType::Player, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Ladder, true);
		CollisionManager::SetLayer(this, eLayerType::Monster, eLayerType::AttackObject, true);
		CollisionManager::SetLayer(this, eLayerType::Monster, eLayerType::Player, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::MonsterAttack, true);
		//CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Portal, true);

	}
	void ValleyScene::Update()
	{
		Scene::Update();
	}
	void ValleyScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void ValleyScene::OnEnterPlayer(UINT _iPlayerID)
	{

	}

	void ValleyScene::OnExitPlayer(UINT _iPlayerID)
	{
		SceneManger::RetrieveAttackObject(_iPlayerID, GetSceneID());
	}

	void ValleyScene::OnEnter()
	{
		
	}
	void ValleyScene::OnExit()
	{
	
	}
	void ValleyScene::CreateBackground()
	{
		/*GameObject* pBackGround = new GameObject();
		pBackGround->SetSceneName(GetName());
		AddGameObject(eLayerType::Background, pBackGround);
		
		pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.2f);
		pBackGround->GetComponent<Transform>()->SetScale(25 * 0.35f,  25.f * 1.f, 1.f);*/

	}


	void ValleyScene::setobject()
	{
		Ladder* pLadder0 = new Ladder();
		pLadder0->SetSceneID(GetSceneID());
	
		pLadder0->GetComponent<Transform>()->SetPosition(0.f, -4.4f, -0.1f);
		pLadder0->GetComponent<Transform>()->SetScale(1.f * 0.6f, 5.5f * 0.6f, 0.f);
		pLadder0->Initialize();
		pLadder0->GetComponent<Collider2D>()->SetCenter(Vector2(0.f, -0.1f));
		AddGameObject(eLayerType::Ladder, pLadder0);

		Ladder* pLadder1 = new Ladder();
		pLadder1->SetSceneID(GetSceneID());
		
		pLadder1->GetComponent<Transform>()->SetPosition(2.f, 4.2f, -0.1f);
		pLadder1->GetComponent<Transform>()->SetScale(1.f * 0.6f, 5.6f * 0.6f, 0.f);
		pLadder1->Initialize();
		AddGameObject(eLayerType::Ladder, pLadder1);

		Ladder* pLadder2 = new Ladder();
		pLadder2->SetSceneID(GetSceneID());
		
		pLadder2->GetComponent<Transform>()->SetPosition(-1.f, -0.7f, -0.1f);
		pLadder2->GetComponent<Transform>()->SetScale(1.f * 0.6f, 4.5f * 0.6f, 0.f);
		pLadder2->Initialize();

		AddGameObject(eLayerType::Ladder, pLadder2);

		Ground* pGround1 = new Ground();
		pGround1->SetSceneID(GetSceneID());
		pGround1->GetComponent<Transform>()->SetPosition(0.f, 0.48f, -0.1f);
		pGround1->GetComponent<Transform>()->SetScale(2.7f * 5.f, 1.f * 0.3f, 0.f);

		AddGameObject(eLayerType::Ground, pGround1);


		Ground* pGround2 = new Ground();
		pGround2->SetSceneID(GetSceneID());
		pGround2->GetComponent<Transform>()->SetPosition(-0.6f, -3.f, -0.1f);
		pGround2->GetComponent<Transform>()->SetScale(2.7f * 1.5f, 1.f * 0.3f, 0.f);

		AddGameObject(eLayerType::Ground, pGround2);

		Ground* pGround3 = new Ground(true);
		pGround3->SetSceneID(GetSceneID());
		pGround3->GetComponent<Transform>()->SetPosition(0.f, -7.6f, -0.1f);
		pGround3->GetComponent<Transform>()->SetScale(2.7f * 5.f, 1.f * 0.3f, 0.f);

		AddGameObject(eLayerType::Ground, pGround3);
	}

	void ValleyScene::setmonster()
	{
		for (int i = 0; i < 5; ++i)
		{
			Effect* pEffect = new Effect();
			pEffect->SetSceneID(GetSceneID());
			pEffect->SetName(L"sklaser_hit");
			pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(134.f, 97.f), 1, 1, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);
		}

		Skelegon* pSkelegon = new Skelegon();
		pSkelegon->SetSceneID(GetSceneID());
		pSkelegon->Initialize();
		
		AddGameObject(eLayerType::Monster, pSkelegon);
	}
}
