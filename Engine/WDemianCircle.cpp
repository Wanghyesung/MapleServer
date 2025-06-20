#include "WDemianCircle.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WMonster.h"
#include "WSceneManger.h"
#include "WRigidbody.h"
#include "WPlayerScript.h"
#include "WBattleManager.h"
#include "WTime.h"
namespace W
{
	UINT DemianCircle::CREATE_ID = 0;

	DemianCircle::DemianCircle():
		m_bEnter(false),
		m_bActive(false),
		m_pTarget(nullptr),
		m_iCurIndex(0),
		m_fCurAttackTime(0.f),
		m_fAttackTime(1.4f)
	{
		AddComponent<Rigidbody>();

		GetComponent<Transform>()->SetScale(4.f, 4.f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"level0", Vector2(0.0f, 0.0f), Vector2(260.f, 272.0f), 12, Vector2(500.f, 500.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"level1", Vector2(0.0f, 272.0f), Vector2(361.f, 374.0f), 11, Vector2(500.f, 500.f), Vector2::Zero, 0.07f);
		pAnim->Create(L"level2", Vector2(0.0f, 646.f), Vector2(431.f, 449.0f), 12, Vector2(500.f, 500.f), Vector2::Zero, 0.07f);

		pAnim->Play(L"level0", true);
	}

	DemianCircle::~DemianCircle()
	{

	}
	void DemianCircle::Initialize()
	{
		m_vecColliderSize.push_back(Vector2(0.85f, 0.85f));
		m_vecColliderSize.push_back(Vector2(1.1f, 1.1f));
		m_vecColliderSize.push_back(Vector2(1.35f, 1.35f));

		Vector3 vPosition = Vector3(0.1f, -1.85f, -1.5f);
		GetComponent<Transform>()->SetPosition(vPosition);

		m_tMonsterAttack.tAttackInfo.fAttackDamage = 3;
		m_tMonsterAttack.tAttackInfo.fAttRcnt = 0.f;//����
		m_tMonsterAttack.tAttackInfo.fAttUpperRcnt = 0.f;

		m_pTarget = SceneManger::FindPlayer(GetSceneName());
	}
	void DemianCircle::Update()
	{
		move();

		check_enter();

		GameObject::Update();
	}
	void DemianCircle::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	
	void DemianCircle::LevelUp(UINT _iLevel)
	{
		m_iCurIndex = _iLevel;
		std::wstring strNum = std::to_wstring(m_iCurIndex);
		GetComponent<Animator>()->Play(L"level"+ strNum, true);
	}
	
	void DemianCircle::check_enter()
	{
		m_fCurAttackTime += Time::DeltaTime();

		for (GameObject* pPlayer : SceneManger::GetPlayers(GetSceneName()))
		{
			Vector3 vPos = pPlayer->GetComponent<Transform>()->GetPosition();
			Vector2 vTargerPos = Vector2(vPos.x, vPos.y);
			Vector3 vPosition = GetComponent<Transform>()->GetPosition();
			Vector2 vCirclePos = Vector2(vPosition.x, vPosition.y);

			float fLen = (vTargerPos - vCirclePos).Length();

			bool bEnter = false;
			if (fabs(fLen) <= (m_vecColliderSize[m_iCurIndex]).Length())
			{
				bEnter = true;
				if (m_fCurAttackTime >= m_fAttackTime)
				{
					pPlayer->GetScript<PlayerScript>()->Hit(m_tMonsterAttack.tAttackInfo, L"Circle");
					m_fCurAttackTime = 0.f;
				}
			}
		}
	}
	void DemianCircle::move()
	{
		
	 	Vector3 vTargetPos = m_pTarget->GetComponent<Transform>()->GetPosition();
		Vector3 vPosition = GetComponent<Transform>()->GetPosition();

		Vector3 vLen = vTargetPos - vPosition;
		int iDir = vLen.x > 0.f ? 1 : -1;

		GetComponent<Rigidbody>()->AddForce(Vector2(iDir * m_fSpeed, 0.f));
	}
}