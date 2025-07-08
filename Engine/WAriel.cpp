#include "WAriel.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WEffect.h"
#include "WMonsterScript.h"
#include "WBossStand.h"
#include "WMonsterAttack.h"
#include "WMonsterDead.h"
#include "WEventManager.h"
#include "WSceneManger.h"
#include "WBossStart.h"
#include "WArielLaser.h"
#include "WArielStone.h"
#include "WBattleManager.h"
#include "WPullObject.h"
#include "WPinkBean.h"
namespace W
{
	UINT Ariel::CREATE_ID = 0;

	Ariel::Ariel()
	{
		SetName(L"Ariel");

		
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Ariel_stand", Vector2(0.f, 0.f), Vector2(400.f, 400.f), 1, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Ariel_start", Vector2(0.f, 400.f), Vector2(400.f, 400.f), 11, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Ariel_attack0", Vector2(0.f, 800.f), Vector2(400.f, 400.f), 16, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Ariel_attack1", Vector2(0.f, 1200.f), Vector2(400.f, 400.f), 23, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Ariel_attack2", Vector2(0.f, 1600.f), Vector2(400.f, 400.f), 14, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Ariel_attack3", Vector2(0.f, 2000.f), Vector2(400.f, 400.f), 17, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Ariel_dead", Vector2(0.f, 2400.f), Vector2(400.f, 400.f), 17, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
	}

	Ariel::~Ariel()
	{
		ObjectPoolManager::ReleaseObject(L"Ariel_attack0");
		ObjectPoolManager::ReleaseObject(L"Ariel_attack1");
	}
	void Ariel::add_skill()
	{
		//1
		for (int i = 0; i < 10; ++i)
		{
			ArielLaser* pLaser = new ArielLaser();
			pLaser->SetSceneID(GetSceneID());
			pLaser->SetName(L"Ariel_attack0");
			AddMonsterSkill(pLaser);
		}
		//2
		for (int i = 0; i < 4; ++i)
		{
			ArielStone* pStone = new ArielStone();
			pStone->SetSceneID(GetSceneID());
			pStone->SetName(L"Ariel_attack1");
			AddMonsterSkill(pStone);
		}
	}

	void Ariel::Initialize()
	{
		GetComponent<Transform>()->SetScale(10.f, 10.f, 0.f);
		GetComponent<Transform>()->SetPosition(0.1f, 0.6f, -1.3f);
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.15f, 0.15f));
		pCollider->SetCenter(Vector2(0.f, 0.f));
		pCollider->SetActive(false);

		MonsterScript* Pscript = AddComponent<MonsterScript>();
		Pscript->SetBoss();
		Pscript->Initialize();

		setattack();
		add_skill();

		MonsterFSM* pFSM = new MonsterFSM();
		pFSM->SetMonster(this);
		Pscript->SetFSM(pFSM);


		pFSM->AddState(new BossStand());
		pFSM->AddState(new BossStart());
		pFSM->AddState(new MonsterAttack());
		pFSM->AddState(new MonsterDead());

		pFSM->SetActiveState(Monster::eMonsterState::start);
		pFSM->Initialize();
	}

	void Ariel::Update()
	{
		GameObject::Update();
	}

	void Ariel::LateUpdate()
	{
		std::wstring strName = GetName();
		std::wstring strState = GetCurStateName();

		if (strState == L"_attack")
		{
			UINT iNumber = GetScript<MonsterScript>()->GetAttackNumber();
			strState += std::to_wstring(iNumber);
		}

		std::wstring strAnimName = GetMonsterAnim();
		std::wstring strCurAnim = strName + strState;

		if (strAnimName != strCurAnim)
		{
			SetMonsterAnim(strCurAnim);
			GetComponent<Animator>()->Play(strCurAnim, true);
		}

		Monster::LateUpdate();
	}
	
	void Ariel::UpdatePacket()
	{
		Monster::UpdatePacket();
	}

	void Ariel::setattack()
	{
		MonsterScript* Pscript = GetComponent<MonsterScript>();
		Vector3 vPosition = GetComponent<Transform>()->GetPosition();

		tMonsterAttack attack1 = {};
		attack1.tTime.fCoolTime = 12.f;
		attack1.bSkill = true;
		attack1.vScale = Vector2(0.f, 0.f);
		attack1.pFunction = std::bind(&Ariel::attack1, this);

		attack1.iStartFrame = 10;
		attack1.iEndFrame = 11;

		Pscript->AddAttack(attack1);

		//2
		tMonsterAttack attack2 = {};
		attack2.tTime.fCoolTime = 6.f;
		attack2.bSkill = true;
		attack2.vScale = Vector2(0.f, 0.f);
		attack2.pFunction = std::bind(&Ariel::attack2, this);

		attack2.iStartFrame = 9;
		attack2.iEndFrame = 10;
	   //
		Pscript->AddAttack(attack2);

		//3
		tMonsterAttack skill = {};
		skill.bSkill = true;
		skill.tTime.fCoolTime = 20.f;

		skill.vScale = Vector2(0.f, 0.f);
		skill.pFunction = std::bind(&Ariel::pull, this);

		skill.iStartFrame = 13;
		skill.iEndFrame = 14;
		Pscript->AddAttack(skill);


		tMonsterAttack skill2 = {};
		skill2.bSkill = true;
		skill2.tTime.fCoolTime = 15.f;

		skill2.vScale = Vector2(0.f, 0.f);
		skill2.pFunction = std::bind(&Ariel::heal, this);

		skill2.iStartFrame = 12;
		skill2.iEndFrame = 13;
		Pscript->AddAttack(skill2);
	}

	void Ariel::attack1()
	{
		//GameObject* pPlayer = SceneManger::FindPlayer();
		Vector3 vStartPos = GetComponent<Transform>()->GetPosition();
		vStartPos.z -= 0.1f;
		vStartPos.y += 0.9f;

		srand(time(NULL));
		for (int i = -3; i < 7; ++i)
		{
			int iIndx = (rand() % 4);

			float fOffsetX = i * 1.5f + vStartPos.x;
			fOffsetX += iIndx * 0.5f;

			MonsterAttackObject* pLaser = GetMonsterSkill(L"Ariel_attack0");
			if (pLaser == nullptr)
				return;
			pLaser->SetOnwer(this);

			pLaser->GetComponent<Transform>()->SetPosition(fOffsetX, vStartPos.y, vStartPos.z);
			pLaser->Initialize();
			EventManager::CreateObject(pLaser, eLayerType::MonsterAttack);
		}
	}

	void Ariel::attack2()
	{
		//GameObject* pPlayer = SceneManger::FindPlayer();
		Vector3 vStartPos = GetComponent<Transform>()->GetPosition();
		vStartPos.z -= 0.1f;
		vStartPos.y -= 0.3f;

		srand(time(NULL));
		for (int i = -2; i < 2; ++i)
		{
			int iIndx = (rand() % 4);

			float fOffsetX = i * 2.3 + vStartPos.x;
			fOffsetX += iIndx * 0.5f;

			MonsterAttackObject* pStone = GetMonsterSkill(L"Ariel_attack1");
			if (pStone == nullptr)
				return;
			pStone->SetOnwer(this);

			pStone->GetComponent<Transform>()->SetPosition(fOffsetX, vStartPos.y, vStartPos.z);
			pStone->Initialize();
			EventManager::CreateObject(pStone, eLayerType::MonsterAttack);
		}
	}

	void Ariel::heal()
	{
		const std::unordered_map<UINT , GameObject*> hashMonster =
			SceneManger::GetActiveScene(this)->GetLayer(eLayerType::Monster)->GetGameObjects();

		float fAccValue = 10.f;

		auto iter = hashMonster.begin();
		for (iter; iter != hashMonster.end(); ++iter)
		{
			GameObject* pMon = iter->second;
			if (dynamic_cast<PinkBean*>(pMon))
				continue;

			MonsterScript* pMonster = pMon->GetScript<MonsterScript>();
			if (pMon->GetState() == GameObject::eState::Active && pMonster->GetObjectInfo().fHP > 0)
				BattleManager::Buff_Stat(pMon, BattleManager::eUpStatType::Heal, fAccValue);
		}			
	}

	void Ariel::pull()
	{
		GameObject* pPlayer = SceneManger::FindPlayerRandom(GetSceneID());
		
		Vector3 vTargetPosition = GetComponent<Transform>()->GetPosition();

		PullObject* pPullObj = new PullObject();
		pPullObj->SetSceneID(GetSceneID());
		pPullObj->SetTarget(pPlayer);
		pPullObj->SetPosition(Vector2(vTargetPosition.x, 0.f));
		pPullObj->SetDeleteTime(1.5f);
		pPullObj->Initialize();

		EventManager::CreateObject(pPullObj, eLayerType::Object);
	}
}