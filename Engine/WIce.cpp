#include "WIce.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WObject.h"
#include "WMonsterAttackScript.h"
#include "WMonsterScript.h"
#include "WMonster.h"
namespace W
{
	Ice::Ice():
		m_iCallCount(0),
		m_tMonsterAttack{}
	{

		Collider2D* pCollider = GetComponent<Collider2D>();
		pCollider->SetActive(false);

		GetComponent<Transform>()->SetScale(2.1f, 2.1f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Ice_left", Vector2(0.0f, 0.0f), Vector2(216.f, 242.0f), 30, Vector2(220.f, 220.f), Vector2::Zero, 0.15f);

		pAnim->CompleteEvent(L"Ice_left") = std::bind(&Ice::off, this);
		pAnim->Play(L"Ice_left", true);

	}

	Ice::~Ice()
	{

	}

	void Ice::Initialize()
	{
		float faccAttack = GetOwner()->GetScript<MonsterScript>()->GetAttackInfo().fAttack;
		m_tMonsterAttack.tAttackInfo.fAttackDamage = 10.f + faccAttack;

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = Vector2(0.4f, 0.4f);
		m_tMonsterAttack.vOffset = Vector2(0.f, -1.f);

		m_tMonsterAttack.iStartFrame = 25;
		m_tMonsterAttack.iEndFrame = 28;
	}

	void Ice::Update()
	{
		GameObject::Update();
	}

	void Ice::LateUpdate()
	{
		Animation* pAnim = GetComponent<Animator>()->GetActiveAnimation();

		int iIndex = pAnim->GetCurIndex();
		if (iIndex >= m_tMonsterAttack.iStartFrame && m_iCallCount ==0)
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
	
	void Ice::off()
	{
		m_iCallCount = 0;

		EventManager::AddMonsterPool(this);

	}
}