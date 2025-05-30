#include "WHorntailRightHand.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonsterAttack.h"
#include "WMonsterFSM.h"
#include "WBossStand.h"
#include "WMonsterScript.h"
#include "WSceneManger.h"
#include "WSpear.h"
#include "WBattleManager.h"
#include "WHorntailDead.h"
#include "WEventManager.h"

namespace W
{
	UINT HorntailRightHand::CREATE_ID = 0;

	HorntailRightHand::HorntailRightHand(Horntail* _pOwner):
		m_pOwner(_pOwner)
	{
		SetName(L"RightHand");

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"RightHand_stand", Vector2(0.f, 0.f), Vector2(500.f, 300.f), 5, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"RightHand_attack0", Vector2(0.f, 300.f), Vector2(500.f, 300.f), 18, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"RightHand_attack1",Vector2(0.f, 600.f), Vector2(500.f, 300.f), 17, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"RightHand_attack2",Vector2(0.f, 900.f), Vector2(500.f, 300.f), 16, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"RightHand_dead", Vector2(0.f, 1200.f), Vector2(500.f, 300.f), 16, Vector2(1000.f, 1000.f), Vector2::Zero, 0.07f);

	}
	HorntailRightHand::~HorntailRightHand()
	{

		ObjectPoolManager::ReleaseObject(L"spear");
	}

	void HorntailRightHand::add_skill()
	{
		//1
		Spear* pSpear = new Spear();
		pSpear->SetName(L"spear");
		pSpear->SetName(GetSceneName());
		AddMonsterSkill(pSpear);
	}


	void HorntailRightHand::Initialize()
	{
		GetComponent<Transform>()->SetScale(11.f, 11.f, 0.f);
		GetComponent<Transform>()->SetPosition(1.23f, -0.9f, -1.5f);
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.2f, 0.2f));
		pCollider->SetCenter(Vector2(-1.57f, 0.f));

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
	void HorntailRightHand::Update()
	{
		Monster::Update();
	}
	void HorntailRightHand::LateUpdate()
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
	
	void HorntailRightHand::setAttack()
	{
		MonsterScript* Pscript = GetComponent<MonsterScript>();

		//1
		tMonsterAttack attack = {};
		attack.tTime.fCoolTime = 12.f;
		attack.bSkill = true;
		attack.vScale = Vector2::Zero;

		attack.iStartFrame = 13;
		attack.iEndFrame = 14;
		attack.pFunction = std::bind(&HorntailRightHand::create_spear, this);

		Pscript->AddAttack(attack);

		//2
		tMonsterAttack attack1 = {};
		attack1.tTime.fCoolTime = 8.f;
		attack1.bSkill = true;
		attack1.vScale = Vector2::Zero;

		attack1.iStartFrame = 12;
		attack1.iEndFrame = 13;

		attack1.pFunction = std::bind(&HorntailRightHand::seal_skill, this);
		Pscript->AddAttack(attack1);

		//3
		tMonsterAttack attack2 = {};
		attack2.tTime.fCoolTime = 15.f;
		attack2.bSkill = true;
		attack2.vScale = Vector2::Zero;

		attack2.iStartFrame = 11;
		attack2.iEndFrame = 12;
		attack2.pFunction = std::bind(&HorntailRightHand::buff_attack, this);
		Pscript->AddAttack(attack2);
	}
	void HorntailRightHand::create_spear()
	{
		GameObject* pGameObj = SceneManger::FindPlayer(GetSceneName());

		if (pGameObj != nullptr)
		{
			Vector3 pPlayerPos = pGameObj->GetComponent<Transform>()->GetPosition();
			Vector3 pPos = GetComponent<Transform>()->GetPosition();

			pPlayerPos.z = 0;
			pPos.z = 0;

			float fLen = (pPlayerPos - pPos).Length();

			if (fLen <= 3.f)
			{
				MonsterAttackObject* pSpear = GetMonsterSkill(L"spear");
				if (pSpear == nullptr)
					return;
				pSpear->SetOnwer(this);
				pSpear->Initialize();
				
				EventManager::CreateObject(pSpear, eLayerType::MonsterAttack);
			}
		}
	}

	void HorntailRightHand::seal_skill()
	{
		
		BattleManager::HitchAbnormal(SceneManger::FindPlayer(GetSceneName()), BattleManager::eAbnormalType::SealSkill);
	}

	void HorntailRightHand::buff_attack()
	{
		if (m_pBuffFunc)
			m_pBuffFunc();
	}

	

}