#include "WSkelegon.h"
#include "WAnimator.h"
#include "WRigidbody.h"
#include "WResources.h"
#include "WMonsterScript.h"

#include "WMonsterFSM.h"
#include "WMonsterAttack.h"
#include "WMonsterStand.h"
#include "WMonsterMove.h"
#include "WMonsterHit.h"
#include "WSceneManger.h"
#include "WEffect.h"
#include "WMonsterDead.h"
#include "ObjectState.pb.h"

namespace W
{
	UINT Skelegon::CREATE_ID = 0;

	Skelegon::Skelegon()
		//m_spAttackEffect(nullptr)
	{
		SetName(L"skelegon");


		Animator* pAnimator = AddComponent<Animator>();
	
		pAnimator->Create(L"skelegon_stand_left", Vector2(0.0f, 0.0f), Vector2(600.0f, 200.0f), 6, Vector2(600.f, 200.f));
		pAnimator->Create(L"skelegon_move_left", Vector2(0.0f, 200.0f), Vector2(600.0f, 200.0f), 4, Vector2(600.f, 200.f));
		pAnimator->Create(L"skelegon_attack0_left", Vector2(0.0f, 400.0f), Vector2(600.0f, 200.0f), 11, Vector2(600.f, 200.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"skelegon_dead_left", Vector2(0.0f, 600.0f), Vector2(600.0f, 200.0f), 8, Vector2(600.f, 200.f), Vector2::Zero, 0.07f);
		pAnimator->Create(L"skelegon_hit_left", Vector2(0.0f, 600.0f), Vector2(600.0f, 200.0f), 1, Vector2(600.f, 200.f));

		pAnimator->Create(L"skelegon_stand_right", Vector2(6000.0f, 0.0f), Vector2(-600.0f, 200.0f), 6, Vector2(600.f, 200.f));
		pAnimator->Create(L"skelegon_move_right", Vector2(6000.0f, 200.0f), Vector2(-600.0f, 200.0f), 4, Vector2(600.f, 200.f));
		pAnimator->Create(L"skelegon_attack0_right", Vector2(6000.0f, 400.0f), Vector2(-600.0f, 200.0f), 11, Vector2(600.f, 200.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"skelegon_dead_right", Vector2(6000.0f, 600.0f), Vector2(-600.0f, 200.0f), 8, Vector2(600.f, 200.f), Vector2::Zero, 0.07f);
		pAnimator->Create(L"skelegon_hit_right", Vector2(6000.0f, 600.0f), Vector2(-600.0f, 200.0f), 1, Vector2(600.f, 200.f));

	
	}

	Skelegon::~Skelegon()
	{

	}
	void Skelegon::Initialize()
	{
		MonsterScript* pSkelegonScript = AddComponent<MonsterScript>();
		pSkelegonScript->Initialize();
		pSkelegonScript->CreateHP();

		tObjectInfo tObjInfo = {};
		//tObjInfo.fAttack = 5.f;
		tObjInfo.fDefense = 5.f;
		tObjInfo.fSpeed = 2.f;
		pSkelegonScript->SetObjectInfo(tObjInfo);


		tMonsterAttack attack = {};
		attack.pFunction = std::bind(&Skelegon::create_laser, this);

		attack.vScale = Vector2(1.8f, 0.5f);
		attack.vOffset = Vector2(1.7f, 0.f);
		attack.tAttackInfo.fAttackDamage = 10.f;

		attack.iStartFrame = 4;
		attack.iEndFrame = 6;
		pSkelegonScript->AddAttack(attack);

		add_skill();

		Collider2D* pCollider = AddComponent<Collider2D>();
		AddComponent<Rigidbody>();
		GetComponent<Transform>()->SetScale(6.f, 2.f, 0.f);
		GetComponent<Transform>()->SetPosition(Vector3(3.f, 1.2f, -1.5f));

		pCollider->SetSize(Vector2(0.4f / 3.f, 0.4f));


		MonsterFSM* pFSM = new MonsterFSM();
		//m_pFSM->SetPlayer(m_pPlayer);
		pFSM->SetMonster(this);
		pFSM->AddState(new MonsterStand());
		pFSM->AddState(new MonsterMove());
		pFSM->AddState(new MonsterHit());
		pFSM->AddState(new MonsterAttack());
		pFSM->AddState(new MonsterDead());

		pFSM->SetActiveState(Monster::eMonsterState::stand);
		pSkelegonScript->SetFSM(pFSM);
		pFSM->Initialize();


		Effect* pEffect = new Effect();
		pEffect->SetSceneName(GetSceneName());
		pEffect->SetName(L"sklaser_hit");
		pEffect->CreateAnimation(Vector2(0.f, 0.f), Vector2(134.f, 97.f), 1, 1, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

		//생성될 아이템
		std::vector<std::wstring> vecItems = { L"alixir",L"10_weapon"};
		SetCreateCount(2);
		SetItem(vecItems);

	}

	void Skelegon::Update()
	{
		Monster::Update();
	}

	void Skelegon::LateUpdate()
	{
		std::wstring strName = GetName();
		std::wstring strState = GetCurStateName();
		std::wstring strDir;

		int iDir = GetDir();
		if (iDir > 0)
			strDir = L"_right";
		else
			strDir = L"_left";

		if (strState == L"_attack")
		{
			UINT iNumber = GetScript<MonsterScript>()->GetAttackNumber();
			strState += std::to_wstring(iNumber);
		}
		std::wstring strAnimName = GetMonsterAnim();
		std::wstring strCurAnim = strName + strState + strDir;

		if (strAnimName != strCurAnim)
		{
			SetMonsterAnim(strCurAnim);
			GetComponent<Animator>()->Play(strCurAnim, true);
		}

		Monster::LateUpdate();

	}

	void Skelegon::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();
		
		update_state();
	}

	//void Skelegon::Render()
	//{
	//	Monster::Render();
	//}

	void Skelegon::add_skill()
	{
		MonsterAttackObject* pLaser = new MonsterAttackObject();
		pLaser->SetSceneName(GetSceneName());
		pLaser->SetName(L"sklaser");
		AddMonsterSkill(pLaser);
	}

	void Skelegon::create_laser()
	{
		MonsterAttackObject* pLaser = GetMonsterSkill(L"sklaser");
		if (pLaser == nullptr)
			return;

		MonsterScript* pMonsterScript = GetScript<MonsterScript>();
		pMonsterScript->SetMonsterAttack(pLaser);
		pLaser->SetOnwer(this);

		EventManager::CreateObject(pLaser, eLayerType::MonsterAttack);
	}
	void Skelegon::update_state()
	{
		Animator* pAnimator = GetComponent<Animator>();

		if (!pAnimator->TrySendPacket())
			return;

		Protocol::S_STATE pkt;
		
		UCHAR cLayer = (UCHAR)eLayerType::Monster;
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cDir = GetDir() > 0 ? 1 : 0; 
		UCHAR cAnimIdx = pAnim->GetCurIndex();
		bool bRender = !IsDead();

		pkt.set_state_value(bRender <<16 | (cDir << 8) | cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneName()));
	
	}
}