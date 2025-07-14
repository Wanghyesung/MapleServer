#include "WPlayerArm.h"
#include "WAnimator.h"
#include "WResources.h"
#include "WPlayerWeapon.h"
namespace W
{
	UINT PlayerArm::CREATE_ID = 0;

	PlayerArm::PlayerArm():
		m_pPlayerWeapon(nullptr)
	{
		
	}
	PlayerArm::~PlayerArm()
	{
		if (m_pPlayerWeapon)
		{
			delete m_pPlayerWeapon;
			m_pPlayerWeapon = nullptr;
		}
	}
	void PlayerArm::Initialize()
	{
		Animator* pAnimator = AddComponent<Animator>();

		pAnimator->Create(L"arm_alert_left", Vector2(0.0f, 0.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f),Vector2::Zero,0.1f);
		pAnimator->Create(L"arm_jump_left", Vector2(0.0f, 150.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_ladder_left", Vector2(0.0f, 300.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_pronestab_left", Vector2(0.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_proneattack_left", Vector2(150.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_stab_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_stand_left", Vector2(0.0f, 750.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_swing1_left", Vector2(0.0f, 900.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_swing2_left", Vector2(0.0f, 1050.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_swingOF_left", Vector2(0.0f, 1200.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_walk_left", Vector2(0.0f, 1350.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"arm_swingQS_left")->Create(L"arm_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"arm_swingQS_left")->Create(L"arm_swingQS_left", Vector2(450.0f, 1200.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);

		pAnimator->Create(L"arm_alert_right", Vector2(450.0f, 0.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_jump_right", Vector2(450.0f, 150.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_ladder_right", Vector2(450.0f, 300.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_pronestab_right", Vector2(450.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_proneattack_right", Vector2(300.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_stab_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_stand_right", Vector2(450.0f, 750.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_swing1_right", Vector2(450.0f, 900.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_swing2_right", Vector2(450.0f, 1050.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_swingOF_right", Vector2(450.0f, 1200.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_walk_right", Vector2(450.0f, 1350.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"arm_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"arm_swingQS_right")->Create(L"arm_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"arm_swingQS_right")->Create(L"arm_swingQS_right", Vector2(0, 1200.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);


		Vector3 vScale = m_pPlayer->GetComponent<Transform>()->GetScale();
		GetComponent<Transform>()->SetScale(vScale);

		m_pPlayerWeapon = new PlayerWeapon();
		m_pPlayerWeapon->SetPlayerArm(this);
	}
	void PlayerArm::Update()
	{
		GameObject::Update();

	}
	void PlayerArm::LateUpdate()
	{
		
	}

	void PlayerArm::SetEquipWeapon(const std::wstring& _strEquipName)
	{
		m_pPlayerWeapon->SetPlayerEquip(_strEquipName);
	}

	const wstring& PlayerArm::GetEquipWeaponName()
	{
		return m_pPlayerWeapon->GetEquipName();
	}

	void PlayerArm::SetAnimationIndex()
	{
		if (m_strCurAnim.size() != 0)
			GetComponent<Animator>()->GetActiveAnimation()->SetIndex(0);
	}

	void PlayerArm::SetStop(bool _bStop)
	{
		GetComponent<Animator>()->Stop(_bStop);
	}

	
}