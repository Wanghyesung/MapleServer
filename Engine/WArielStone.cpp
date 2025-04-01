#include "WArielStone.h"
#include "WResources.h"
#include "WAnimator.h"

#include "WEventManager.h"
#include "WMonsterAttackScript.h"
#include "WMonsterScript.h"
#include "WMonster.h"
namespace W
{
	ArielStone::ArielStone():
		m_iCallCount(0),
		m_tMonsterAttack{}
	{
		
		Collider2D* pCollider = GetComponent<Collider2D>();
		pCollider->SetActive(false);

		GetComponent<Transform>()->SetScale(6.f, 6.f, 0.f);

		//std::shared_ptr<Texture> pAtlas = Resources::Find<Texture>(L"Ariel_attack1");
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Stone", Vector2(0.0f, 0.0f), Vector2(306.f, 512.0f), 18, Vector2(510.f, 510.f), Vector2::Zero, 0.15f);

		pAnim->CompleteEvent(L"Stone") = std::bind(&ArielStone::off, this);
		pAnim->Play(L"Stone", true);

	}
	ArielStone::~ArielStone()
	{
	}

	void ArielStone::Initialize()
	{
		float faccAttack = GetOwner()->GetScript<MonsterScript>()->GetAttackInfo().fAttack;
		m_tMonsterAttack.tAttackInfo.fAttackDamage = 10.f + faccAttack;
		m_tMonsterAttack.tAttackInfo.fAttUpperRcnt = 0.f;
		m_tMonsterAttack.tAttackInfo.fAttRigidityTime = 0.5f;
		m_tMonsterAttack.tAttackInfo.fAttRcnt = 5.f;
		m_tMonsterAttack.tAttackInfo.eAttType = eAttackType::SuperKnockback;

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = (Vector2(0.25f, 0.2f));
		m_tMonsterAttack.vOffset = Vector2(0.f, -2.2f);

		m_tMonsterAttack.iStartFrame = 12;
		m_tMonsterAttack.iEndFrame = 15;
	}
	void ArielStone::Update()
	{
		GameObject::Update();
	}
	void ArielStone::LateUpdate()
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

	void ArielStone::off()
	{
		m_iCallCount = 0;
		EventManager::AddMonsterPool(this);
	}
}