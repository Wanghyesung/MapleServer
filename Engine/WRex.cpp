#include "WRex.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WEffect.h"
#include "WMonsterScript.h"
#include "WBossStand.h"
#include "WMonsterAttack.h"
#include "WMonsterDead.h"
#include "WEventManager.h"
#include "WSceneManger.h"
#include "WRexThunder.h"
#include "WBossStart.h"

namespace W
{
	UINT Rex::CREATE_ID = 0;
	Rex::Rex()
	{
		SetName(L"Rex");

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Rex_stand", Vector2(0.f, 0.f), Vector2(500.f, 1000.f), 1, Vector2(1000.f, 1000.f), Vector2::Zero, 0.15f);
		pAnim->Create(L"Rex_start", Vector2(0.f, 1000.f), Vector2(500.f, 1000.f), 11, Vector2(1000.f, 1000.f), Vector2::Zero, 0.15f);
		pAnim->Create(L"Rex_attack0", Vector2(0.f, 2000.f), Vector2(500.f, 1000.f), 18, Vector2(1000.f, 1000.f), Vector2::Zero, 0.15f);
		pAnim->Create(L"Rex_attack1", Vector2(0.f, 3000.f), Vector2(500.f, 1000.f), 20, Vector2(1000.f, 1000.f), Vector2::Zero, 0.15f);
		pAnim->Create(L"Rex_attack2", Vector2(0.f, 4000.f), Vector2(500.f, 1000.f), 20, Vector2(1000.f, 1000.f), Vector2::Zero, 0.15f);
		pAnim->Create(L"Rex_dead", Vector2(0.f, 5000.f), Vector2(500.f, 1000.f), 15, Vector2(1000.f, 1000.f), Vector2::Zero, 0.15f);

	}
	Rex::~Rex()
	{
		ObjectPoolManager::ReleaseObject(L"Rex_attack0");
		ObjectPoolManager::ReleaseObject(L"Rex_attack1");
		ObjectPoolManager::ReleaseObject(L"Rex_attack2");
	}

	void Rex::add_skill()
	{
		//1
		MonsterAttackObject* attack1 = new MonsterAttackObject();
		attack1->SetSceneName(GetSceneName());
		attack1->SetName(L"Rex_attack0");
		AddMonsterSkill(attack1);
		//2
		RexThunder* pThunder = new RexThunder();
		pThunder->SetSceneName(GetSceneName());
		pThunder->SetName(L"Rex_attack1");
		AddMonsterSkill(pThunder);
		//3
		MonsterAttackObject* attack3 = new MonsterAttackObject();
		attack3->SetSceneName(GetSceneName());
		attack3->SetName(L"Rex_attack2");
		AddMonsterSkill(attack3);
	}

	void Rex::Initialize()
	{
		GetComponent<Transform>()->SetScale(10.f, 10.f, 0.f);
		GetComponent<Transform>()->SetPosition(5.55f, 2.05f, -1.3f);
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.2f, 0.5f));
		pCollider->SetCenter(Vector2(0.4f, -1.8f));
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
	void Rex::Update()
	{
		Monster::Update();
	}
	void Rex::LateUpdate()
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
	void Rex::setattack()
	{
		MonsterScript* Pscript = GetComponent<MonsterScript>();
		Vector3 vPosition = GetComponent<Transform>()->GetPosition();

		tMonsterAttack attack1 = {};
		attack1.tTime.fCoolTime = 12.f;
		attack1.pFunction = std::bind(&Rex::attack1, this);

		attack1.tAttackInfo.fAttRcnt = 2.f;

		attack1.vPosition = vPosition;
		attack1.vScale = Vector2(8.f, 2.f);
		attack1.vOffset = Vector2(0.f, -4.f);

		attack1.iStartFrame = 12;
		attack1.iEndFrame = 14;
		attack1.tAttackInfo.fAttackDamage = 10.f;
		Pscript->AddAttack(attack1);

		//2
		tMonsterAttack attack2 = {};
		attack2.tTime.fCoolTime = 6.f;
		attack2.bSkill = true;
		attack2.vScale = Vector2(0.f, 0.f);
		attack2.pFunction = std::bind(&Rex::attack2, this);

		attack2.iStartFrame = 9;
		attack2.iEndFrame = 10;

		Pscript->AddAttack(attack2);

		//3
		tMonsterAttack attack3 = {};

		attack3.tTime.fCoolTime = 9.f;

		attack3.vPosition = vPosition;
		attack3.vScale = Vector2(8.f, 2.f);
		attack3.vOffset = Vector2(0.f, -4.f);

		attack3.pFunction = std::bind(&Rex::attack3, this);
		attack3.iStartFrame = 8;
		attack3.iEndFrame = 11;
		attack3.tAttackInfo.fAttackDamage = 10.f;

		Pscript->AddAttack(attack3);
	}

	void Rex::attack1()
	{
		MonsterAttackObject* attack1 = GetMonsterSkill(L"Rex_attack0");
		if (attack1 == nullptr)
			return;

		MonsterScript* pMonsterScript = GetScript<MonsterScript>();
		pMonsterScript->SetMonsterAttack(attack1);
		attack1->SetOnwer(this);

		EventManager::CreateObject(attack1, eLayerType::MonsterAttack);
	}
	void Rex::attack2()
	{
		GameObject* pGameObj = SceneManger::FindPlayer(GetSceneName());

		if (pGameObj != nullptr)
		{
			Vector3 pPlayerPos = pGameObj->GetComponent<Transform>()->GetPosition();
			Vector3 pPos = GetComponent<Transform>()->GetPosition();

			pPlayerPos.z = 0;
			pPos.z = 0;

			float fLen = (pPlayerPos - pPos).Length();

			if (fLen <= 5.f)
			{
				MonsterAttackObject* pThunder = GetMonsterSkill(L"Rex_attack1");
				if (pThunder == nullptr)
					return;;

				pThunder->SetOnwer(this);
				pThunder->Initialize();
				EventManager::CreateObject(pThunder, eLayerType::MonsterAttack);
			}
		}
	}
	void Rex::attack3()
	{
		MonsterAttackObject* attack3 = GetMonsterSkill(L"Rex_attack2");
		if (attack3 == nullptr)
			return;;

		MonsterScript* pMonsterScript = GetScript<MonsterScript>();
		pMonsterScript->SetMonsterAttack(attack3);
		attack3->SetOnwer(this);

		EventManager::CreateObject(attack3, eLayerType::MonsterAttack);
	}

	void Rex::dark()
	{

	}

	void Rex::attack_reflect()
	{

	}
}