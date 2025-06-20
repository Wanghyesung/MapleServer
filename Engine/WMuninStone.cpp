#include "WMuninStone.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonsterAttackScript.h"
#include "WMonsterScript.h"
#include "WMonster.h"
namespace W
{
	UINT MuninStone::CREATE_ID = 0;

	MuninStone::MuninStone() :
		m_iCallCount(0),
		m_tMonsterAttack{}
	{
		
		Collider2D* pCollider = GetComponent<Collider2D>();
		pCollider->SetActive(false);
		
		GetComponent<Transform>()->SetScale(6.f, 6.f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Stone_left", Vector2(0.0f, 0.0f), Vector2(145.f, 514.0f), 20, Vector2(510.f, 510.f), Vector2::Zero, 0.07f);

		pAnim->CompleteEvent(L"Stone_left") = std::bind(&MuninStone::off, this);
		pAnim->Play(L"Stone_left", true);

	}
	MuninStone::~MuninStone()
	{
	}
	void MuninStone::Initialize()
	{
		float faccAttack = GetOwner()->GetScript<MonsterScript>()->GetAttackInfo().fAttack;
		m_tMonsterAttack.tAttackInfo.fAttackDamage = 10.f + faccAttack;

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = (Vector2(0.25f, 0.2f));
		m_tMonsterAttack.vOffset = Vector2(0.f, -2.1f);

		m_tMonsterAttack.iStartFrame = 15;
		m_tMonsterAttack.iEndFrame = 16;
	}
	void MuninStone::Update()
	{
		GameObject::Update();
	}
	void MuninStone::LateUpdate()
	{
		Animation* pAnim = GetComponent<Animator>()->GetActiveAnimation();

		int iIndex = pAnim->GetCurIndex();
		if (iIndex >= m_tMonsterAttack.iStartFrame && m_iCallCount == 0)
		{
			m_iCallCount = 1;
			//지금 실행중인 데미지 정보 넘기기
			SetMonsterAttack(m_tMonsterAttack);
		}

		if (m_iCallCount == 1 && iIndex >= m_tMonsterAttack.iEndFrame)
		{
			m_iCallCount = 2;
			GetComponent<Collider2D>()->SetActive(false);
		}

		GameObject::LateUpdate();
	}

	void MuninStone::off()
	{
		m_iCallCount = 0;
		EventManager::AddMonsterPool(this);
	}
}