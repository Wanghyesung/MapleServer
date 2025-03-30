#include "WFire.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WObject.h"
#include "WMonsterAttackScript.h"
#include "WMonster.h"
namespace W
{
	Fire::Fire() :
		m_iCallCount(0),
		m_tMonsterAttack{}
	{

		Collider2D* pCollider = GetComponent<Collider2D>();
		pCollider->SetActive(false);

		GetComponent<Transform>()->SetScale(1.1f, 1.1f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Fire_left", Vector2(0.0f, 0.0f), Vector2(111.f, 110.0f), 30, Vector2(110.f, 110.f), Vector2::Zero, 0.15f);

		pAnim->CompleteEvent(L"Fire_left") = std::bind(&Fire::off, this);
		pAnim->Play(L"Fire_left", true);

	}

	Fire::~Fire()
	{

	}

	void Fire::Initialize()
	{
		m_tMonsterAttack.tAttackInfo.fAttackDamage = 10.f;

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = Vector2(0.6f, 0.6f);
		m_tMonsterAttack.vOffset = Vector2(0.f, -0.25f);

		m_tMonsterAttack.iStartFrame = 25;
		m_tMonsterAttack.iEndFrame = 27;
	}

	void Fire::Update()
	{
		GameObject::Update();
	}

	void Fire::LateUpdate()
	{
		Animation* pAnim = GetComponent<Animator>()->GetActiveAnimation();

		int iIndex = pAnim->GetCurIndex();
		if (iIndex >= m_tMonsterAttack.iStartFrame && m_iCallCount == 0)
		{
			m_iCallCount = 1;
			//���� �������� ������ ���� �ѱ��
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
	
	void Fire::off()
	{
		m_iCallCount = 0;
		
		EventManager::AddMonsterPool(this);
	}
}