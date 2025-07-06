#include "WDemianFire0.h"
#include "WMonster.h"
#include "WSceneManger.h"
#include "WRigidbody.h"
#include "WAnimator.h"
#include "WBattleManager.h"
#include "WMonsterAttackScript.h"

namespace W
{
	UINT DemianFire0::CREATE_ID = 0;

	DemianFire0::DemianFire0():
		m_bMove(false),
		m_bEnd(false),
		m_strCurAnim{},
		m_vVelocity(Vector2::Zero)
	{
		AddComponent<Rigidbody>();
		GetComponent<Collider2D>()->SetActive(false);

		//기본적으로 제공하는 script를 제거하고 전용 script로

		GetComponent<Transform>()->SetScale(18.f, 18.f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"fire_start", Vector2(0.0f, 0.0f), Vector2(144.f, 142.0f), 9, Vector2(2000.f, 2000.f), Vector2::Zero, 0.06f);
		pAnim->Create(L"fire_end", Vector2(0.0f, 142.f), Vector2(385.f, 209.0f), 7, Vector2(2000.f, 2000.f), Vector2::Zero, 0.05f);

		pAnim->CompleteEvent(L"fire_start") = std::bind(&DemianFire0::start, this);
		pAnim->CompleteEvent(L"fire_end") = std::bind(&DemianFire0::off, this);

	}
	DemianFire0::~DemianFire0()
	{

	}
	void DemianFire0::Initialize()
	{
		m_tMonsterAttack.tAttackInfo.fAttackDamage = BattleManager::GetMaxDamage();

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = Vector2(0.06f, 0.06f);
		m_tMonsterAttack.vOffset = Vector2(0.f, 0.f);

		SetMonsterAttack(m_tMonsterAttack);

		m_vVelocity = GetComponent<Rigidbody>()->GetVelocity();
	}
	void DemianFire0::Update()
	{
		if (m_bMove)
			move();

		if (!m_bEnd)
		{
			m_bEnd = GetScript<MonsterAttackScript>()->IsEnter();

			if (m_bEnd)
			{
				m_bMove = false;
				
			}
				
		}

		GameObject::Update();
	}
	void DemianFire0::LateUpdate()
	{
		Animator* pAnimator = GetComponent<Animator>();

		std::wstring strState = L"_start";

		if (m_bEnd)
			strState = L"_end";

		else if (m_bMove)
			strState = L"_move";


		std::wstring strAnim = L"fire" + strState;

		if (m_strCurAnim != strAnim)
		{
			m_strCurAnim = strAnim;
			pAnimator->Play(strAnim, true);
		}

		GameObject::LateUpdate();
	}

	void DemianFire0::UpdatePacket()
	{
		MonsterAttackObject::UpdatePacket();
	}
	
	void DemianFire0::off()
	{
		m_bMove = false;
		m_bEnd = false;
		GetScript<MonsterAttackScript>()->SetEnter(false);
		GetScript<MonsterAttackScript>()->InitStack();

		EventManager::AddMonsterPool(this);
	}
	void DemianFire0::start()
	{
		m_bMove = true;
	}

	void DemianFire0::move()
	{
		Rigidbody* pRigidbody = GetComponent<Rigidbody>();
		pRigidbody->AddForce(m_vVelocity);
	}
}