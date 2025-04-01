#include "WRoccatBean.h"
#include "WResources.h"
#include "WStoneScript.h"
#include "WAnimator.h"
#include "WMonsterScript.h"
#include "WMonster.h"
namespace W
{
	RoccatBean::RoccatBean() :
		m_iCallCount(0),
		m_tMonsterAttack{}
	{

		Collider2D* pCollider = GetComponent<Collider2D>();
		pCollider->SetActive(false);

		GetComponent<Transform>()->SetScale(1.5f, 1.5f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"attack3", Vector2(0.0f, 0.0f), Vector2(109.f, 159.0f), 6, Vector2(150.f, 150.f), Vector2::Zero, 0.15f);

		pAnim->CompleteEvent(L"attack3") = std::bind(&RoccatBean::off, this);
		pAnim->Play(L"attack3", true);

	}

	RoccatBean::~RoccatBean()
	{

	}
	void RoccatBean::Initialize()
	{
		float faccAttack = GetOwner()->GetScript<MonsterScript>()->GetAttackInfo().fAttack;
		m_tMonsterAttack.tAttackInfo.fAttackDamage = 10.f + faccAttack;

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = (Vector2(0.6f, 0.6f));
		m_tMonsterAttack.vOffset = Vector2(0.f, 0.f);

		m_tMonsterAttack.iStartFrame = 1;
		m_tMonsterAttack.iEndFrame = 3;
	}
	void RoccatBean::Update()
	{
		GameObject::Update();
	}
	void RoccatBean::LateUpdate()
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
	
	void RoccatBean::off()
	{	
		m_iCallCount = 0;
		EventManager::AddMonsterPool(this);
		
	}
}