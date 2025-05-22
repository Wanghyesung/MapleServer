#include "WDemianScene.h"
#include "WResources.h"

#include "WGround.h"
#include "WDemian.h"
#include "WCollisionManager.h"
#include "WDemianPhase2.h"
#include "WSharHP.h"
#include "WMonsterScript.h"
#include "WWhite.h"
#include "WEventManager.h"
#include "WStigmaBack.h"
#include "WTime.h"
#include "WThreadPool.h"
#include "WMonsterManager.h"
#include "WSceneManger.h"
namespace W
{
	DemianScene::DemianScene():
		m_pWhite(nullptr),
		m_bEnd(false),
		m_pStigmaBack(nullptr),
		m_iFadeCallStack(1),
		m_fStigmaTime(30.f),
		m_fCurStigmaTime(0.f)
	{
		SetMapSize(-1.9f, -1.f, 1.9f, -1.f);
		SetMapPossibleSize(-8.9f, 8.9f);

	}
	DemianScene::~DemianScene()
	{
		if (!m_bEnd)
		{
			delete m_pWhite;
			m_pWhite = nullptr;
		}
	}
	void DemianScene::Initialize()
	{
		CreateBackground();
		create_effect();
		create_monster();
		create_event();

		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Ground, true);
		CollisionManager::SetLayer(this, eLayerType::ItemObject, eLayerType::Ground, true);
		CollisionManager::SetLayer(this, eLayerType::ItemObject, eLayerType::Player, true);
		CollisionManager::SetLayer(this, eLayerType::Monster, eLayerType::AttackObject, true);
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::MonsterAttack, true);
		CollisionManager::SetLayer(this, eLayerType::Ground, eLayerType::MonsterAttack, true);

		m_bEnd = false;
		m_iFadeCallStack = 1;

	}
	void DemianScene::Update()
	{
		float fRatio = 0.f;
		float fMaxHP = 0.f;
		float fHP = 0.f;

		const std::unordered_map<UINT, GameObject*> hashMonster = GetLayer(eLayerType::Monster)->GetGameObjects();
		
		auto iter = hashMonster.begin();
		for (iter; iter != hashMonster.end(); ++iter)
		{
			GameObject* pMon = iter->second;
			if (pMon->GetState() == GameObject::eState::Paused)
			{
				if (m_iFadeCallStack == 1 &&
					dynamic_cast<Demian*>(pMon))
				{
					fadein();
				}
			}

			const tObjectInfo& tInfo = pMon->GetScript<MonsterScript>()->GetObjectInfo();

			fMaxHP += tInfo.fMaxHP;
			fHP += tInfo.fHP;
		}

		fRatio = (fHP / fMaxHP) * 100.f;
		m_pSharHP->SetHPValue(fRatio);

		Scene::Update();
	}
	void DemianScene::LateUpdate()
	{
		Scene::LateUpdate();

	}
	
	void DemianScene::OnEnter()
	{
		Scene::OnEnter();
		ThreadPool::Joinable();

	}
	void DemianScene::OnExit()
	{
		Scene::OnExit();

		m_pWhite = nullptr;
	}
	void DemianScene::CreateBackground()
	{
		//GameObject* pBackGround = new GameObject();
		//pBackGround->SetSceneName(GetName());
		//AddGameObject(eLayerType::Background, pBackGround);
		//
		//pBackGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 1.f);
		////2 : 1
		//pBackGround->GetComponent<Transform>()->SetScale(2.2372f * 8.f, 1.f * 8.f, 1.f);


		Ground* pGround = new Ground(true);
		pGround->SetSceneName(GetName());
		pGround->GetComponent<Transform>()->SetPosition(0.f, -2.95f, -0.1f);
		pGround->GetComponent<Transform>()->SetScale(4.3f * 7.f, 1.f * 0.3f, 0.f);
		AddGameObject(eLayerType::Ground, pGround);
	}

	void DemianScene::fadein()
	{
		m_bEnd = true;
		m_iFadeCallStack = 0;
		m_pWhite->Initialize();
		EventManager::CreateObject(m_pWhite, eLayerType::Object);
	}
	void DemianScene::phase2()
	{
		m_bEnd = false;
		std::unordered_map<UINT, GameObject*> hashMonster = GetLayer(eLayerType::Monster)->GetGameObjects();

		//phase2 µ¥¹Ì¾È
		hashMonster[LAYER_STARAT_IDX + 1]->GetComponent<Collider2D>()->SetActive(true);
		hashMonster[LAYER_STARAT_IDX + 1]->SetState(GameObject::eState::Active);
	}

	void DemianScene::check_stigma()
	{
		m_fCurStigmaTime += Time::DeltaTime();
		if (m_fCurStigmaTime >= m_fStigmaTime)
		{
			m_fCurStigmaTime = 0.f;
			GameObject* pPlayer = SceneManger::FindPlayer(GetName());
			BattleManager::HitchAbnormal(pPlayer,BattleManager::eAbnormalType::Stigma);
		}
	}

	void DemianScene::create_monster()
	{
		Demian* pDemian = new Demian();
		pDemian->SetSceneName(GetName());
		pDemian->Initialize();
		AddGameObject(eLayerType::Monster, pDemian);

		DemianPhase2* pDemian2 = new DemianPhase2();
		pDemian2->SetSceneName(GetName());
		pDemian2->Initialize();
		pDemian2->GetComponent<Collider2D>()->SetActive(false);
		pDemian2->SetState(GameObject::eState::Paused);
		AddGameObject(eLayerType::Monster, pDemian2);
	}

	void DemianScene::create_event()
	{
		m_pWhite = new White();
		m_pWhite->SetSceneName(GetName());
		m_pWhite->SetEndTime(2.f);
		m_pWhite->SetFunction(std::bind(&DemianScene::phase2, this));

		m_pSharHP = new SharHP();
		m_pSharHP->SetSceneName(GetName());
		m_pSharHP->Initialize();
		AddGameObject(eLayerType::UI, m_pSharHP);

		m_pStigmaBack = new StigmaBack();
		m_pStigmaBack->SetSceneName(GetName());
		m_pStigmaBack->Initialize();
		AddGameObject(eLayerType::Object, m_pStigmaBack);
	}

	void DemianScene::create_effect()
	{
		
		Effect* pAttack_hit1 = new Effect();
		pAttack_hit1->SetSceneName(GetName());
		pAttack_hit1->SetName(L"Demian2_attack1");
		pAttack_hit1->CreateAnimation(Vector2(0.f, 0.f), Vector2(227.f, 228.f), 5, 1, Vector2(250.f, 250.f), Vector2(0.f, 0.f), 0.2f);

		Effect* pEffect = new Effect();
		pEffect->SetSceneName(GetName());
		pEffect->GetComponent<Transform>()->SetScale(Vector3(18.f, 18.f, 0.f));
		pEffect->SetName(L"DemianAttack0Effect");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(1122.f, 382.f), 9, 1, Vector2(2000.f, 2000.f), Vector2(0.f, 0.f), 0.1f);


		for (int i = 0; i < 2; ++i)
		{
			Effect* pClone1 = new Effect();
			pClone1->SetSceneName(GetName());
			pClone1->GetComponent<Transform>()->SetScale(Vector3(18.f, 18.f, 0.f));
			pClone1->SetName(L"DemianClone0");
			
			pClone1->CreateAnimation(Vector2(0.f, 0.f), Vector2(515.f, 318.f), 8, 1, Vector2(2000.f, 2000.f), Vector2(0.f, 0.f), 0.1f);
		}
		
		Effect* pClone2 = new Effect();
		pClone2->SetSceneName(GetName());
		pClone2->GetComponent<Transform>()->SetScale(Vector3(18.f, 18.f, 0.f));
		pClone2->SetName(L"DemianClone1");
	
		pClone2->CreateAnimation(Vector2(0.f, 0.f), Vector2(679.f, 542.f), 8, 1, Vector2(1800.f, 1800.f), Vector2(0.f, 0.f), 0.1f);
	
		for (int i = 0; i < 3; ++i)
		{
			Effect* pEffect = new Effect();
			pEffect->SetSceneName(GetName());
			pEffect->GetComponent<Transform>()->SetScale(3.5f, 3.5f, 0.f);
			pEffect->SetName(L"DemianTarget");
			pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(286.f, 288.f), 14, 1, Vector2(300.f, 300.f), Vector2::Zero, 0.1f);
		}
	}
}