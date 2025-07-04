#include "WRexThunder.h"
#include "WSceneManger.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonster.h"

namespace W
{
	UINT RexThunder::CREATE_ID = 0;

	RexThunder::RexThunder() :
		m_pTarget(nullptr)
	{
		
		Collider2D* pCollider = GetComponent<Collider2D>();

		GetComponent<Transform>()->SetScale(8.f, 8.f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Thunder_left", Vector2(0.0f, 0.0f), Vector2(174.f, 821.0f), 5, Vector2(800.f, 800.f), Vector2::Zero, 0.15f);

		pAnim->CompleteEvent(L"Thunder_left") = std::bind(&RexThunder::off, this);
		pAnim->Play(L"Thunder_left", true);

	}

	RexThunder::~RexThunder()
	{

	}

	void RexThunder::Initialize()
	{
		m_tMonsterAttack.tAttackInfo.fAttackDamage = 10.f;

		m_tMonsterAttack.vScale = Vector2(0.2f, 0.2f);
		m_tMonsterAttack.vOffset = Vector2(0.f, -3.f);

		m_tMonsterAttack.iStartFrame = 0;
		m_tMonsterAttack.iEndFrame = 4;

		m_pTarget = SceneManger::FindPlayerRandom(GetSceneID());
	}
	void RexThunder::Update()
	{
		if (m_pTarget != nullptr)
		{
			Vector3 vPos = GetComponent<Transform>()->GetPosition();
			Vector3 vPlayerPos = m_pTarget->GetComponent<Transform>()->GetPosition();

			vPlayerPos.y += 3.3f;
			vPlayerPos.z -= 0.5f;
			GetComponent<Transform>()->SetPosition(vPlayerPos);
		}

		GameObject::Update();
	}
	void RexThunder::LateUpdate()
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

	void RexThunder::UpdatePacket()
	{
		MonsterAttackObject::UpdatePacket();
	}

	void RexThunder::off()
	{
		m_iCallCount = 0;
		EventManager::AddMonsterPool(this);
	}
}