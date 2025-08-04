#include "WHorntailHeadB.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonsterScript.h"
#include "WBossStand.h"
#include "WMonsterAttack.h"
#include "WMonsterFSM.h"
#include "WFire.h"
#include "WSceneManger.h"
#include "WEffect.h"
#include "WBattleManager.h"
#include "WHorntailDead.h"
#include "WEventManager.h"

namespace W
{
	UINT HorntailHeadB::CREATE_ID = 0;

	HorntailHeadB::HorntailHeadB(Horntail* _pOwner) :
		m_pOwner(_pOwner)
	{
		SetName(L"HeadB");

	
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"HeadB_stand", Vector2(0.f, 0.f), Vector2(500.f, 800.f), 10, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"HeadB_attack0", Vector2(0.f, 800.f), Vector2(500.f, 800.f), 26, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"HeadB_attack1", Vector2(0.f, 1600.f), Vector2(500.f, 800.f), 15, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"HeadB_attack2", Vector2(0.f, 2400.f), Vector2(500.f, 800.f), 13, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"HeadB_dead", Vector2(0.f, 3200.f), Vector2(500.f, 800.f), 14, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);

	}

	HorntailHeadB::~HorntailHeadB()
	{
		ObjectPoolManager::ReleaseObject(L"firebreath");
		ObjectPoolManager::ReleaseObject(L"fire");
	}

	void HorntailHeadB::add_skill()
	{
		//1
		MonsterAttackObject* breath = new MonsterAttackObject();
		breath->SetSceneID(GetSceneID());
		breath->SetName(L"firebreath");
		AddMonsterSkill(breath);
		//2
		for (int i = 0; i < 8; ++i)
		{
			Fire* pFire = new Fire();
			pFire->SetSceneID(GetSceneID());
			pFire->SetName(L"fire");
			AddMonsterSkill(pFire);
		}
	}

	void HorntailHeadB::Initialize()
	{
		GetComponent<Transform>()->SetScale(11.f, 11.f, 0.f);
		GetComponent<Transform>()->SetPosition(-1.53f, -0.05f, -1.6f);
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.2f, 0.2f));
		pCollider->SetCenter(Vector2(0.f, 1.f));

		//보스 등록
		MonsterScript* Pscript = AddComponent<MonsterScript>();
		Pscript->SetBoss();
		Pscript->Initialize();

		setAttack();
		add_skill();

		MonsterFSM* pFSM = new MonsterFSM();
		pFSM->SetMonster(this);
		Pscript->SetFSM(pFSM);

		BossStand* pHorntailStand = new BossStand();
		pFSM->AddState(pHorntailStand);
		MonsterAttack* pMonsterAttack = new MonsterAttack();
		pFSM->AddState(pMonsterAttack);
		HorntailDead* pDead = new HorntailDead();
		pFSM->AddState(pDead);

		pFSM->SetActiveState(Monster::eMonsterState::stand);
		pFSM->Initialize();

	}

	void HorntailHeadB::Update()
	{
		Monster::Update();
	}

	void HorntailHeadB::LateUpdate()
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

	void HorntailHeadB::UpdatePacket()
	{
		Monster::UpdatePacket();
	}

	void HorntailHeadB::setAttack()
	{
		MonsterScript* Pscript = GetComponent<MonsterScript>();
		Vector3 vPosition = GetComponent<Transform>()->GetPosition();

		//1
		tMonsterAttack breath = {};
		breath.tTime.fCoolTime = 5.f;

		breath.pFunction = std::bind(&HorntailHeadB::create_breath, this);

		breath.vScale = Vector2(2.3f, 6.f);
		breath.vOffset = Vector2(0.f, -3.f);

		breath.vPosition = vPosition;
		breath.iStartFrame = 18;
		breath.iEndFrame = 23;
		breath.tAttackInfo.fAttackDamage = 10.f;
		Pscript->AddAttack(breath);

		//2
		tMonsterAttack attack1 = {};
		attack1.bSkill = true;
		attack1.tTime.fCoolTime = 9.f;

		attack1.vScale = Vector2::Zero;

		attack1.iStartFrame = 6;
		attack1.iEndFrame = 7;

		attack1.pFunction = std::bind(&HorntailHeadB::create_fire, this);
		Pscript->AddAttack(attack1);

		//3
		tMonsterAttack attack2 = {};
		attack2.bSkill = true;
		attack2.tTime.fCoolTime = 15.f;

		attack2.vScale = Vector2::Zero;

		attack2.iStartFrame = 5;
		attack2.iEndFrame = 6;
		attack2.pFunction = std::bind(&HorntailHeadB::faint, this);
		Pscript->AddAttack(attack2);
	}

	void HorntailHeadB::create_breath()
	{
		MonsterAttackObject* breath = GetMonsterSkill(L"firebreath");
		if (breath == nullptr)
			return;

		MonsterScript* pMonsterScript = GetScript<MonsterScript>();
		pMonsterScript->SetMonsterAttack(breath);
		breath->SetOnwer(this);
		

		EventManager::CreateObject(breath, eLayerType::MonsterAttack);
	}

	void HorntailHeadB::create_fire()
	{
		Vector3 vStartPos = Vector3(-6.f, -3.14f, -1.5f);

		for (UINT i = 0; i < 8; ++i)
		{
			float fOffsetX = i * 2 + vStartPos.x;

			MonsterAttackObject* pFire = GetMonsterSkill(L"fire");
			if (pFire == nullptr)
				return;
			pFire->SetOnwer(this);

			pFire->GetComponent<Transform>()->SetPosition(fOffsetX, vStartPos.y, vStartPos.z);
			pFire->Initialize();
			EventManager::CreateObject(pFire, eLayerType::MonsterAttack);
		}
	}

	void HorntailHeadB::faint()
	{
		BattleManager::HitchAbnormal(SceneManger::FindPlayerRandom(GetSceneID()),BattleManager::eAbnormalType::Faint);
	}

	
}