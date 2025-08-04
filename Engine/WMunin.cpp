#include "WMunin.h"
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
#include "WMuninStone.h"
namespace W
{
	UINT Munin::CREATE_ID = 0;

	Munin::Munin()
	{
		SetName(L"Munin");

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Munin_stand",  Vector2(0.f, 0.f), Vector2(350.f, 500.f), 1, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Munin_start",  Vector2(0.f, 500.f), Vector2(350.f, 500.f), 11, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Munin_attack0", Vector2(0.f, 1000.f), Vector2(350.f, 500.f), 16, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Munin_attack1", Vector2(0.f, 1500.f), Vector2(350.f, 500.f), 23, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Munin_attack2", Vector2(0.f, 2000.f), Vector2(350.f, 500.f), 14, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Munin_attack3", Vector2(0.f, 2500.f), Vector2(350.f, 500.f), 17, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);
		pAnim->Create(L"Munin_dead", Vector2(0.f, 3000.f), Vector2(350.f, 500.f), 17, Vector2(1000.f, 1000.f), Vector2::Zero, 0.075f);

	}
	Munin::~Munin()
	{
		ObjectPoolManager::ReleaseObject(L"Munin_attack0");
		ObjectPoolManager::ReleaseObject(L"Munin_attack1");
	}

	void Munin::add_skill()
	{
		//1
		MonsterAttackObject* attack1 = new MonsterAttackObject();
		attack1->SetSceneID(GetSceneID());
		attack1->SetName(L"Munin_attack0");
		AddMonsterSkill(attack1);
		//2
		MuninStone* pStone = new MuninStone();
		pStone->SetSceneID(GetSceneID());
		pStone->SetName(L"Munin_attack1");
		pStone->SetOnwer(this);
		AddMonsterSkill(pStone);
	}

	void Munin::Initialize()
	{
		GetComponent<Transform>()->SetScale(10.f, 10.f, 0.f);
		GetComponent<Transform>()->SetPosition(2.7f, -0.75f, -1.3f);
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.1f, 0.2f));
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
	void Munin::Update()
	{
		GameObject::Update();
	}
	void Munin::LateUpdate()
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
	
	void Munin::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		Animator* pAnimator = GetComponent<Animator>();

		bool bRender = IsDead();

		Protocol::S_STATE pkt;
		UCHAR cLayer = (UCHAR)eLayerType::Monster;
		UCHAR cSceneID = (UCHAR)GetSceneID();
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | (cSceneID << 16) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cDir = 1;
		UCHAR cAnimIdx = pAnim->GetCurIndex();

		pkt.set_state_value(!bRender << 16 | (cDir << 8) | cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneID()));
	}

	void Munin::setattack()
	{
		MonsterScript* Pscript = GetComponent<MonsterScript>();
		Vector3 vPosition = GetComponent<Transform>()->GetPosition();

		tMonsterAttack attack1 = {};
		attack1.tTime.fCoolTime = 12.f;
		attack1.pFunction = std::bind(&Munin::attack1, this);

		attack1.tAttackInfo.fAttRcnt = 2.f;

		attack1.vPosition = vPosition;
		attack1.vScale = Vector2(6.f, 2.f);
		attack1.vOffset = Vector2(0.f, -0.5f);

		attack1.iStartFrame = 11;
		attack1.iEndFrame = 13;
		attack1.tAttackInfo.fAttackDamage = 10.f;
		Pscript->AddAttack(attack1);

		//2
		tMonsterAttack attack2 = {};
		attack2.tTime.fCoolTime = 6.f;
		attack2.bSkill = true;
		attack2.vScale = Vector2(0.f, 0.f);
		attack2.pFunction = std::bind(&Munin::attack2, this);

		attack2.iStartFrame = 13;
		attack2.iEndFrame = 14;

		Pscript->AddAttack(attack2);

		//3
		tMonsterAttack skill = {};
		skill.bSkill = true;
		skill.tTime.fCoolTime = 18.f;
		skill.tTime.fCurTime = 17.f;

		skill.vScale = Vector2(0.f, 0.f);
		skill.pFunction = std::bind(&Munin::attack_reflect, this);

		skill.iStartFrame = 8;
		skill.iEndFrame = 9;
		Pscript->AddAttack(skill);


		tMonsterAttack skill2 = {};
		skill2.bSkill = true;
		skill2.tTime.fCoolTime = 15.f;

		skill2.vScale = Vector2(0.f, 0.f);
		skill2.pFunction = std::bind(&Munin::dark, this);

		skill2.iStartFrame = 6;
		skill2.iEndFrame = 7;
		Pscript->AddAttack(skill2);

	}
	void Munin::attack1()
	{
		MonsterAttackObject* attack1 = GetMonsterSkill(L"Munin_attack0");
		if (attack1 == nullptr)
			return;

		MonsterScript* pMonsterScript = GetScript<MonsterScript>();
		pMonsterScript->SetMonsterAttack(attack1);
		attack1->SetOnwer(this);

		EventManager::CreateObject(attack1, eLayerType::MonsterAttack);
	}
	void Munin::attack2()
	{
	 	GameObject* pPlayer = SceneManger::FindPlayerRandom(GetSceneID());
		Vector3 vPosisiton = pPlayer->GetComponent<Transform>()->GetPosition();
		vPosisiton.z -= 0.1f;
		vPosisiton.y += 2.5f;

		MonsterAttackObject* pStone = GetMonsterSkill(L"Munin_attack1");
		if (pStone == nullptr)
			return;
		pStone->SetOnwer(this);

		pStone->GetComponent<Transform>()->SetPosition(vPosisiton);
		pStone->Initialize();
		EventManager::CreateObject(pStone, eLayerType::MonsterAttack);
	}
	

	void Munin::dark()
	{

	}
	void Munin::attack_reflect()
	{
		BattleManager::Buff_Stat(this, BattleManager::eUpStatType::ReflexAttack, 1.f);
	}
}