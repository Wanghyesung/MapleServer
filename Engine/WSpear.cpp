#include "WSpear.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonsterAttackScript.h"
#include "WSceneManger.h"
#include "WMonster.h"
namespace W
{
	UINT Spear::CREATE_ID = 0;

	Spear::Spear():
		m_pTarget(nullptr)
	{
		
		Collider2D* pCollider = GetComponent<Collider2D>();
		pCollider->SetActive(false);

		GetComponent<Transform>()->SetScale(2.1f, 2.1f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Spear_left", Vector2(0.0f, 0.0f), Vector2(185.f, 160.0f), 17, Vector2(220.f, 220.f), Vector2::Zero, 0.07f);

		pAnim->CompleteEvent(L"Spear_left") = std::bind(&Spear::off, this);
		pAnim->Play(L"Spear_left", true);
	}

	Spear::~Spear()
	{

	}

	void Spear::Initialize()
	{
		m_tMonsterAttack.tAttackInfo.fAttackDamage = 10.f;

		m_tMonsterAttack.vPosition = GetComponent<Transform>()->GetPosition();
		m_tMonsterAttack.vScale = Vector2(0.6f, 0.6f);
		m_tMonsterAttack.vOffset = Vector2(0.f, 0.f);

		m_tMonsterAttack.iStartFrame = 11;
		m_tMonsterAttack.iEndFrame = 12;

		m_pTarget = SceneManger::FindPlayer(GetSceneName());
	}

	void Spear::Update()
	{
		if (m_pTarget != nullptr)
		{
			Vector3 vPos = GetComponent<Transform>()->GetPosition();
			Vector3 vPlayerPos = m_pTarget->GetComponent<Transform>()->GetPosition();

			vPlayerPos.z -= 0.5f;
			GetComponent<Transform>()->SetPosition(vPlayerPos);
		}

		GameObject::Update();
	}
	void Spear::LateUpdate()
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
	
	void Spear::off()
	{
		m_iCallCount = 0;
		EventManager::AddMonsterPool(this);
	}
}