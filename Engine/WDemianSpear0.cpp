#include "WDemianSpear0.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WObject.h"
#include "WMonsterAttackScript.h"
#include "WMonster.h"
#include "WBattleManager.h"
namespace W
{
	DemianSpear0::DemianSpear0() :
		m_iCallCount(0),
		m_tMonsterAttack{}
	{
		

		Collider2D* pCollider = GetComponent<Collider2D>();
		pCollider->SetActive(false);

		GetComponent<Transform>()->SetScale(18.f, 18.f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Spear", Vector2(0.0f, 0.0f), Vector2(258.f, 538.0f), 29, Vector2(2000.f, 2000.f), Vector2::Zero, 0.15f);

		pAnim->CompleteEvent(L"Spear") = std::bind(&DemianSpear0::off, this);
		pAnim->Play(L"Spear", true);

	}

	DemianSpear0::~DemianSpear0()
	{

	}

	void DemianSpear0::Initialize()
	{
		m_tMonsterAttack.tAttackInfo.fAttackDamage = BattleManager::GetMaxDamage();

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = Vector2(0.05f, 0.2f);
		m_tMonsterAttack.vOffset = Vector2(0.f, 0.f);

		int iDir = GetOwner()->GetDir();
		Vector3 vRotate = Vector3(0.f, 0.f, iDir * XM_PI / 7.f);
		m_tMonsterAttack.vRoatate = vRotate;
		GetComponent<Transform>()->SetRotation(vRotate);

		m_tMonsterAttack.iStartFrame = 19;
		m_tMonsterAttack.iEndFrame = 23;
	}

	void DemianSpear0::Update()
	{
		GameObject::Update();
	}

	void DemianSpear0::LateUpdate()
	{
		Animation* pAnim = GetComponent<Animator>()->GetActiveAnimation();

		int iIndex = pAnim->GetCurIndex();
		if (iIndex >= m_tMonsterAttack.iStartFrame && m_iCallCount == 0)
		{
			m_iCallCount = 1;
			//지금 실행중인 데미지 정보 넘기기
			SetMonsterAttack(m_tMonsterAttack);
			GetComponent<Collider2D>()->SetActive(true);
		}

		if (m_iCallCount == 1 && iIndex >= m_tMonsterAttack.iEndFrame)
		{
			m_iCallCount = 2;
			GetComponent<Collider2D>()->SetActive(false);
		}

		GameObject::LateUpdate();
	}
	
	void DemianSpear0::off()
	{
		m_iCallCount = 0;
		GetScript<MonsterAttackScript>()->InitStack();
		EventManager::AddMonsterPool(this);
	}
}