#include "WHorntailTail.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonsterScript.h"
#include "WMonsterFSM.h"
#include "WBossStand.h"
#include "WMonsterAttack.h"
#include "WEffect.h"
#include "WSceneManger.h"
#include "WHorntail.h"
#include "WHorntailDead.h"
namespace W
{
	UINT HorntailTail::CREATE_ID = 0;

	HorntailTail::HorntailTail(Horntail* _pOwner):
		m_pOwner(_pOwner)
	{
		SetName(L"Tail");

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Tail_stand", Vector2(0.f, 0.f), Vector2(550, 450.f), 1, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"Tail_attack0", Vector2(0.f, 450.f), Vector2(550, 450.f), 21, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"Tail_dead", Vector2(0.f, 900.f), Vector2(550, 450.f), 10, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		
	}
	HorntailTail::~HorntailTail()
	{
		ObjectPoolManager::ReleaseObject(L"tailattack");

	}
	void HorntailTail::Initialize()
	{
		GetComponent<Transform>()->SetScale(11.f, 11.f, 0.f);
		GetComponent<Transform>()->SetPosition(1.42f, -1.57f, -1.2f);
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.3f, 0.2f));
		pCollider->SetCenter(Vector2(0.f, -1.f));

		//���� ���
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

	void HorntailTail::add_skill()
	{
		//1
		MonsterAttackObject* attack1 = new MonsterAttackObject();
		attack1->SetSceneName(GetSceneName());
		attack1->SetName(L"tailattack");
		AddMonsterSkill(attack1);
	}

	void HorntailTail::Update()
	{
		Monster::Update();
	}
	void HorntailTail::LateUpdate()
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
	
	void HorntailTail::setAttack()
	{
		MonsterScript* Pscript = GetComponent<MonsterScript>();

		//1
		tMonsterAttack attack0 = {};
		attack0.tTime.fCoolTime = 5.f;

		attack0.pFunction = std::bind(&HorntailTail::attack, this);

		attack0.vScale = Vector2(3.3f, 2.f);
		attack0.vOffset = Vector2(1.f, -1.5f);

		attack0.iStartFrame = 14;
		attack0.iEndFrame = 16;
		attack0.tAttackInfo.fAttackDamage = 10.f;
		Pscript->AddAttack(attack0);

	
	}
	void HorntailTail::attack()
	{
		MonsterAttackObject* attack1 = GetMonsterSkill(L"tailattack");
		if (attack1 == nullptr)
			return;

		MonsterScript* pMonsterScript = GetScript<MonsterScript>();
		pMonsterScript->SetMonsterAttack(attack1);
		attack1->SetOnwer(this);

		EventManager::CreateObject(attack1, eLayerType::MonsterAttack);
	}
}