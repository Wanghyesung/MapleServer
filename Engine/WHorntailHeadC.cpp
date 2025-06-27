#include "WHorntailHeadC.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonsterScript.h"
#include "WBossStand.h"
#include "WMonsterAttack.h"
#include "WMonsterFSM.h"
#include "WThunder.h"
#include "WSceneManger.h"
#include "WEffect.h"
#include "WBattleManager.h"
#include "WHorntailDead.h"
#include "WEventManager.h"
namespace W
{
	UINT HorntailHeadC::CREATE_ID = 0;

	HorntailHeadC::HorntailHeadC(Horntail* _pOwner) :
		m_pOwner(_pOwner)
	{
		SetName(L"HeadC");

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"HeadC_stand", Vector2(0.f, 0.f), Vector2(600.f, 700.f), 10, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"HeadC_attack0", Vector2(0.f, 700.f), Vector2(600.f, 700.f), 23, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"HeadC_attack1", Vector2(0.f, 1400.f), Vector2(600.f, 700.f), 18, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"HeadC_attack2", Vector2(0.f, 2100.f), Vector2(600.f, 700.f), 14, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"HeadC_dead", Vector2(0.f, 2800.f), Vector2(600.f, 700.f), 13, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		
	}

	HorntailHeadC::~HorntailHeadC()
	{
		ObjectPoolManager::ReleaseObject(L"thunderbreath");
		ObjectPoolManager::ReleaseObject(L"thunder");
	}

	void HorntailHeadC::add_skill()
	{
		//1
		MonsterAttackObject* breath = new MonsterAttackObject();
		breath->SetName(L"thunderbreath");
		breath->SetSceneName(GetSceneName());
		AddMonsterSkill(breath);

		//2
		for (int i = 0; i < 8; ++i)
		{
			Thunder* pThunder = new Thunder();
			pThunder->SetName(L"thunder");
			pThunder->SetSceneName(GetSceneName());
			AddMonsterSkill(pThunder);
		}
	}

	void HorntailHeadC::Initialize()
	{
		GetComponent<Transform>()->SetScale(11.f, 11.f, 0.f);
		GetComponent<Transform>()->SetPosition(1.68f, -1.55f, -1.5f);
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.2f, 0.2f));
		pCollider->SetCenter(Vector2(-1.4f, 2.2f));

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

	void HorntailHeadC::Update()
	{
		Monster::Update();
	}

	void HorntailHeadC::LateUpdate()
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

	void HorntailHeadC::UpdatePacket()
	{
		Monster::UpdatePacket();
	}

	void HorntailHeadC::setAttack()
	{
		MonsterScript* Pscript = GetComponent<MonsterScript>();
		Vector3 vPosition = GetComponent<Transform>()->GetPosition();

		//1
		tMonsterAttack breath = {};
		breath.tTime.fCoolTime = 12.f;

		breath.pFunction = std::bind(&HorntailHeadC::create_breath, this);

		breath.vScale = Vector2(2.3f, 6.f);
		breath.vOffset = Vector2(1.7f, -0.7f);
		breath.vRoatate = Vector3(0.f, 0.f, XM_PI / 2.f);

		breath.vPosition = vPosition;
		breath.iStartFrame = 13;
		breath.iEndFrame = 20;
		breath.tAttackInfo.fAttackDamage = 10.f;
		Pscript->AddAttack(breath);

		//2
		tMonsterAttack attack1 = {};
		attack1.bSkill = true;
		attack1.tTime.fCoolTime = 8.f;

		attack1.vScale = Vector2::Zero;

		attack1.iStartFrame = 8;
		attack1.iEndFrame = 9;

		attack1.pFunction = std::bind(&HorntailHeadC::create_thunder, this);
		Pscript->AddAttack(attack1);

		//3
		tMonsterAttack attack2 = {};
		attack2.bSkill = true;
		attack2.tTime.fCoolTime = 15.f;

		attack2.vScale = Vector2::Zero;

		attack2.iStartFrame = 7;
		attack2.iEndFrame = 8;
		attack2.pFunction = std::bind(&HorntailHeadC::ignore, this);
		Pscript->AddAttack(attack2);
	}

	void HorntailHeadC::create_breath()
	{
		MonsterAttackObject* breath = GetMonsterSkill(L"thunderbreath");
		if (breath == nullptr)
			return;


		MonsterScript* pMonsterScript = GetScript<MonsterScript>();
		pMonsterScript->SetMonsterAttack(breath);
		breath->SetOnwer(this);
		
		EventManager::CreateObject(breath, eLayerType::MonsterAttack);
	}



	void HorntailHeadC::create_thunder()
	{
		Vector3 vStartPos = Vector3(6.5f, -0.65f, -1.9f);

		for (int i = 0; i < 8; ++i)
		{
			float fOffsetX = -i * 2 + vStartPos.x;

			MonsterAttackObject* pThunder = GetMonsterSkill(L"thunder");
		
			pThunder->SetOnwer(this);

			pThunder->GetComponent<Transform>()->SetPosition(fOffsetX, vStartPos.y, vStartPos.z);
			pThunder->Initialize();
			EventManager::CreateObject(pThunder, eLayerType::MonsterAttack);
		}
	}

	void HorntailHeadC::ignore()
	{
		BattleManager::Buff_Stat(this, BattleManager::eUpStatType::Ignore, 1);
	}

}