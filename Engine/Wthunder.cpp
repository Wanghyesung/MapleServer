#include "WThunder.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonsterAttackScript.h"
#include "WMonster.h"

namespace W
{
	UINT Thunder::CREATE_ID = 0;

	Thunder::Thunder() :
		m_iCallCount(0),
		m_tMonsterAttack{}
	{
		

		Collider2D* pCollider = GetComponent<Collider2D>();
		pCollider->SetActive(false);

		GetComponent<Transform>()->SetScale(6.f, 6.f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Thunder_left", Vector2(0.0f, 0.0f), Vector2(180.f, 845.0f), 30, Vector2(800.f, 800.f), Vector2::Zero, 0.07f);

		pAnim->CompleteEvent(L"Thunder_left") = std::bind(&Thunder::off, this);
		pAnim->Play(L"Thunder_left", true);

	}

	Thunder::~Thunder()
	{

	}

	void Thunder::Initialize()
	{
		m_tMonsterAttack.tAttackInfo.fAttackDamage = 10.f;

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = Vector2(0.15f, 1.f);
		m_tMonsterAttack.vOffset = Vector2(0.f, -0.25f);

		m_tMonsterAttack.iStartFrame = 25;
		m_tMonsterAttack.iEndFrame = 29;
	}

	void Thunder::Update()
	{
		GameObject::Update();
	}

	void Thunder::LateUpdate()
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
	
	void Thunder::UpdatePacket()
	{
		MonsterAttackObject::UpdatePacket();
	}

	void Thunder::off()
	{
		m_iCallCount = 0;
		EventManager::AddMonsterPool(this);
	}
}