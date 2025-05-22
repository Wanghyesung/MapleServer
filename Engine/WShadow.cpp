#include "WShadow.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WPlayer.h"
#include "WPlayerScript.h"
#include "WTime.h"
#include "WSceneManger.h"
namespace W
{
	UINT Shadow::CREATE_ID = 0;

	Shadow::Shadow() :
		m_pOwner(nullptr),
		m_pFuncAttack(nullptr),
		m_fActiveTime(0.f),
		m_fCurTime(0.f),
		m_fAttackTime(0.f),
		m_fCurAttackTime(0.f),
		m_bActive(false),
		m_bOff(false),
		m_strCurAnim{}
	{
		Animator* pAnimator = AddComponent<Animator>();
		pAnimator->Create(L"_alert_left", Vector2(0.0f, 0.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_jump_left", Vector2(0.0f, 150.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_ladder_left", Vector2(0.0f, 300.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_pronestab_left", Vector2(0.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_proneattack_left", Vector2(150.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);

		pAnimator->Create(L"_stab_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_stand_left", Vector2(0.0f, 750.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_swing1_left", Vector2(0.0f, 900.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_swing2_left", Vector2(0.0f, 1050.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_swingOF_left", Vector2(0.0f, 1200.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_walk_left", Vector2(0.0f, 1350.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);
		pAnimator->FindAnimation(L"_swingQS_left")->Create(L"_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);
		pAnimator->FindAnimation(L"_swingQS_left")->Create(L"_swingQS_left", Vector2(450.0f, 1200.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);

		pAnimator->Create(L"_alert_right", Vector2(450.0f, 0.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_jump_right", Vector2(450.0f, 150.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_ladder_right", Vector2(450.0f, 300.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_pronestab_right", Vector2(450.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_proneattack_right", Vector2(300.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);

		pAnimator->Create(L"_stab_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_stand_right", Vector2(450.0f, 750.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_swing1_right", Vector2(450.0f, 900.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_swing2_right", Vector2(450.0f, 1050.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_swingOF_right", Vector2(450.0f, 1200.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_walk_right", Vector2(450.0f, 1350.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(L"_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);
		pAnimator->FindAnimation(L"_swingQS_right")->Create(L"_swingQS_right",Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);
		pAnimator->FindAnimation(L"_swingQS_right")->Create(L"_swingQS_right",Vector2(0, 1200.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);

		pAnimator->Create(L"_dead", Vector2(150.0f, 1500.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);

		pAnimator->Play(L"_stand_left", true);
		pAnimator->CompleteEvent(L"_dead") = std::bind(&Shadow::off, this);
		GetComponent<Transform>()->SetScale(Vector3(1.5f, 1.5f, 0.f));
	}

	Shadow::~Shadow()
	{

	}
	void Shadow::Initialize()
	{
		
	}

	void Shadow::Update()
	{
		if (!m_bActive)
			return;

		m_fCurTime += Time::DeltaTime();

		PlayerScript* pScript = m_pOwner->GetComponent<PlayerScript>();
		/*pScript->GetObjectInfo().fHP <= 0.f ||*/
		if (m_fCurTime >= m_fActiveTime)
		{
			m_bOff = true;
			m_pOwner->SetShadow(false);
		}
		
		if (m_pFuncAttack)
			update_attack();

		GameObject::Update();
	}

	void Shadow::LateUpdate()
	{
		if (!m_bActive)
			return;

		if (m_pOwner->IsAnimStop())
			GetComponent<Animator>()->Stop(true);
		else
			GetComponent<Animator>()->Stop(false);

		Animator* pAnimator = GetComponent<Animator>();
		Vector3 vPlayerPos = m_pOwner->GetComponent<Transform>()->GetPosition();

		int iDir = m_pOwner->GetDir();
		vPlayerPos.z += 0.01f;
		if (m_pOwner->GetCurPlayerState() == Player::ePlayerState::ladder)
			vPlayerPos.y -= 0.4f;
		else
			vPlayerPos.x -= iDir * 0.4f;
		GetComponent<Transform>()->SetPosition(vPlayerPos);
		
		std::wstring strDir;
		std::wstring strState;
		if (iDir > 0)
			strDir = L"_right";
		else
			strDir = L"_left";
		
		strState = m_pOwner->GetCurStateName();
		std::wstring strAnim = strState + strDir;
		if (m_bOff)
			strAnim = L"_dead";

		if (m_strCurAnim != strAnim)
		{
			m_strCurAnim = strAnim;
			bool bLoop = m_pOwner->IsLoop();
			pAnimator->Play(strAnim, bLoop);
		}

		GameObject::LateUpdate();
	}

	void Shadow::update_attack()
	{
		m_fCurAttackTime += Time::DeltaTime();
		if (m_fCurAttackTime >= m_fAttackTime)
		{
			m_fCurAttackTime = 0.f;
			m_pFuncAttack();
			m_pFuncAttack = nullptr;
		}
	}
	void Shadow::off()
	{
		m_fCurTime = 0.f;
		//SceneManger::Erase(this);
		m_bOff = false;
		m_bActive = false;
	}
}