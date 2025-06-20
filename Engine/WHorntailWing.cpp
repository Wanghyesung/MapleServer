#include "WHorntailWing.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonsterScript.h"
#include "WMonsterFSM.h"
#include "WBossStand.h"
#include "WMonsterAttack.h"
#include "WHorntailDead.h"
namespace W
{
	UINT HorntailWing::CREATE_ID = 0;

	HorntailWing::HorntailWing(Horntail* _pOwner):
		m_pOwner(_pOwner)
	{
		SetName(L"Wing");

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Wing_stand", Vector2(0.f, 0.f), Vector2(1000.f, 600.f), 8, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"Wing_attack0", Vector2(0.f, 600.f), Vector2(1000.f, 600.f), 10, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->FindAnimation(L"Wing_attack0")->Create(L"Wing_attack0", Vector2(0.f, 1200.f), Vector2(1000.f, 600.f), 11, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);

		pAnim->Create(L"Wing_attack1", Vector2(0.f, 1800.f), Vector2(1000.f, 600.f), 10, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->FindAnimation(L"Wing_attack1")->Create(L"Wing_attack1", Vector2(0.f, 2400.f), Vector2(1000.f, 600.f), 9, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"Wing_dead", Vector2(0.f, 3000.f), Vector2(1000.f, 600.f), 15, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
	}

	HorntailWing::~HorntailWing()
	{
		ObjectPoolManager::ReleaseObject(L"tailattack");


	}
	void HorntailWing::Initialize()
	{
		GetComponent<Transform>()->SetScale(11.f, 11.f, 0.f);
		GetComponent<Transform>()->SetPosition(-1.5f, -0.27f, -1.2f);
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.45f, 0.15f));
		pCollider->SetCenter(Vector2(0.f, -0.5f));

		//���� ���
		MonsterScript* Pscript = AddComponent<MonsterScript>();
		Pscript->SetBoss();
		Pscript->Initialize();

		setAttack();

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
	void HorntailWing::Update()
	{
		Monster::Update();

	}
	void HorntailWing::LateUpdate()
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
	
	void HorntailWing::setAttack()
	{
		MonsterScript* Pscript = GetComponent<MonsterScript>();

		//1
		tMonsterAttack attack0 = {};
		attack0.tTime.fCoolTime = 10.f;
		attack0.bSkill = true;
		attack0.vScale = Vector2::Zero;

		attack0.iStartFrame = 11;
		attack0.iEndFrame = 12;
		attack0.pFunction = std::bind(&HorntailWing::Heal, this);
		Pscript->AddAttack(attack0);

		//2
		tMonsterAttack attack1 = {};
		attack1.bSkill = true;
		attack1.tTime.fCoolTime = 30.f;

		attack1.vScale = Vector2::Zero;

		attack1.iStartFrame = 11;
		attack1.iEndFrame = 12;
		attack1.pFunction = std::bind(&HorntailWing::summons, this);
		Pscript->AddAttack(attack1);

	}
	void HorntailWing::summons()
	{

	}

	void HorntailWing::Heal()
	{
		if (m_pHealFunc)
			m_pHealFunc();
	}
}