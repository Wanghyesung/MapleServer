#include "WMegnusStone.h"
#include "WMonsterAttackScript.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WRigidbody.h"
#include "WCollider2D.h"
#include "WBattleManager.h"
#include "WMonster.h"
#include "WSceneManger.h"
namespace W
{
	UINT MegnusStone::CREATE_ID = 0;

	MegnusStone::MegnusStone() :
		m_bMove(false),
		m_bEnd(false),
		m_strCurAnim{}
	{
		AddComponent<Rigidbody>();
		AddComponent<Collider2D>()->SetActive(false);

		
		GetComponent<Transform>()->SetScale(10.f, 10.f, 0.f);


		Animator* pAnim = AddComponent<Animator>();
	
		pAnim->Create(L"stone_start", Vector2(0.0f, 0.0f), Vector2(350.0f, 350.0f), 7, Vector2(1100.f, 1100.f), Vector2::Zero, 0.12f);
		pAnim->Create(L"stone_move", Vector2(0.0f, 350.0f), Vector2(350.0f, 350.0f), 8, Vector2(1100.f, 1100.f), Vector2::Zero, 0.12f);
		pAnim->Create(L"stone_end", Vector2(0.0f, 700.f), Vector2(350.0f, 350.0f), 6, Vector2(1100.f, 1100.f), Vector2::Zero, 0.1f);

		pAnim->CompleteEvent(L"stone_start") = std::bind(&MegnusStone::start, this);
		pAnim->CompleteEvent(L"stone_end") = std::bind(&MegnusStone::off, this);

		//mr->SetMaterial(pMater);
	}
	MegnusStone::~MegnusStone()
	{

	}
	void MegnusStone::Initialize()
	{
		m_tMonsterAttack.tAttackInfo.fAttackDamage = BattleManager::GetMaxDamage();

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = Vector2(0.12f, 0.12f);
		m_tMonsterAttack.vOffset = Vector2(0.f, 0.f);
	}
	void MegnusStone::Update()
	{
		if (m_bMove)
			move();

		if (!m_bEnd)
		{
			m_bEnd = GetScript<MonsterAttackScript>()->IsEnter();

			if (m_bEnd)
				m_bMove = false;
		}

		GameObject::Update();
	}
	void MegnusStone::LateUpdate()
	{
		Animator* pAnimator = GetComponent<Animator>();

		std::wstring strState = L"_start";

		if (m_bEnd)
			strState = L"_end";

		else if (m_bMove)
			strState = L"_move";


		std::wstring strAnim = L"stone" + strState;

		if (m_strCurAnim != strAnim)
		{
			m_strCurAnim = strAnim;
			pAnimator->Play(strAnim, true);
		}

		GameObject::LateUpdate();
	}
	
	void MegnusStone::move()
	{
		Rigidbody* pRigidbody = GetComponent<Rigidbody>();
		pRigidbody->AddForce(Vector2(0.f, -1.8f));
	}
	void MegnusStone::start()
	{
		m_bMove = true;
		SetMonsterAttack(m_tMonsterAttack);
	}
	void MegnusStone::off()
	{
		m_bMove = false;
		m_bEnd = false;
		GetScript<MonsterAttackScript>()->SetEnter(false);
		GetScript<MonsterAttackScript>()->InitStack();
		
		EventManager::AddMonsterPool(this);
	}
}