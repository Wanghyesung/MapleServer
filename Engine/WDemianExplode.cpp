#include "WDemianExplode.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WBattleManager.h"

#include "WSceneManger.h"
#include "WMonster.h"
#include "WMonsterAttackScript.h"

namespace W
{
	UINT DemianExplode::CREATE_ID = 0;

	DemianExplode::DemianExplode():
		m_iCallCount(0),
		m_tMonsterAttack{}

	{
		

		Collider2D* pCollider = GetComponent<Collider2D>();
		pCollider->SetActive(false);

		GetComponent<Transform>()->SetScale(4.f, 4.f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Explode", Vector2(0.0f, 0.0f), Vector2(439.f, 432.0f), 36, Vector2(440.f, 440.f), Vector2::Zero, 0.15f);

		pAnim->CompleteEvent(L"Explode") = std::bind(&DemianExplode::off, this);
		pAnim->Play(L"Explode", true);

	}
	DemianExplode::~DemianExplode()
	{

	}
	void DemianExplode::Initialize()
	{
		m_tMonsterAttack.tAttackInfo.fAttackDamage = BattleManager::GetMaxDamage();

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = Vector2(0.6f,0.7f);
		m_tMonsterAttack.vOffset = Vector2(0.f, -0.2f);

		m_tMonsterAttack.iStartFrame = 29;
		m_tMonsterAttack.iEndFrame = 30;
	}
	void DemianExplode::Update()
	{
		GameObject::Update();
	}
	void DemianExplode::LateUpdate()
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
	
	void DemianExplode::off()
	{
		m_iCallCount = 0;
		GetScript<MonsterAttackScript>()->InitStack();

		EventManager::AddMonsterPool(this);
	
	}
}