#include "WPlayerHat.h"
#include "WAnimator.h"
#include "WResources.h"
#include "WPlayerHead.h"

namespace W
{
	PlayerHat::PlayerHat():
		m_pPlayerHead(nullptr),
		m_strCurAnim{},
		m_strCurEquip{}
	{
		
		Animator* pAnimator = AddComponent<Animator>();
	}

	PlayerHat::~PlayerHat()
	{

	}
	void PlayerHat::Initialize()
	{
		if (m_strCurEquip.size() == 0)
			return;

		Animator* pAnimator = GetComponent<Animator>();

		pAnimator->Create(m_strCurEquip + L"_alert_left", Vector2(0.0f, 0.0f), Vector2(150.0f, 150.0f), 3 ,Vector2(120.f,120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_jump_left", Vector2(0.0f, 150.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_ladder_left", Vector2(0.0f, 300.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_pronestab_left", Vector2(0.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_proneattack_left", Vector2(150.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_stab_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_stand_left", Vector2(0.0f, 750.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_swing1_left", Vector2(0.0f, 900.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_swing2_left", Vector2(0.0f, 1050.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_swingOF_left", Vector2(0.0f, 1200.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_walk_left", Vector2(0.0f, 1350.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_left")->Create(m_strCurEquip + L"_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_left")->Create(m_strCurEquip + L"_swingQS_left", Vector2(450.0f, 1200.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);


		pAnimator->Create(m_strCurEquip + L"_alert_right", Vector2(450.0f, 0.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_jump_right", Vector2(450.0f, 150.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_ladder_right", Vector2(450.0f, 300.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_pronestab_right", Vector2(450.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_proneattack_right", Vector2(300.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_stab_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_stand_right", Vector2(450.0f, 750.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_swing1_right", Vector2(450.0f, 900.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_swing2_right", Vector2(450.0f, 1050.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_swingOF_right", Vector2(450.0f, 1200.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_walk_right", Vector2(450.0f, 1350.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
		pAnimator->Create(m_strCurEquip + L"_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_right")->Create(m_strCurEquip + L"_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_right")->Create(m_strCurEquip + L"_swingQS_right", Vector2(0, 1200.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);


		Vector3 vScale = m_pPlayerHead->GetComponent<Transform>()->GetScale();
		GetComponent<Transform>()->SetScale(vScale);
	}
	void PlayerHat::Update()
	{
		if (m_strCurEquip.size() == 0)
			return;
		GameObject::Update();
	}
	void PlayerHat::LateUpdate()
	{
		if (m_strCurEquip.size() == 0)
			return;

		Animator* pAnimator = GetComponent<Animator>();
		Vector3 vPlayerPos = m_pPlayerHead->GetComponent<Transform>()->GetPosition();
		vPlayerPos.z -= 0.02f;
		GetComponent<Transform>()->SetPosition(vPlayerPos);

		Player* pPlayer = m_pPlayerHead->GetPlayer();
		int iDir = pPlayer->GetDir();
		std::wstring strDir;
		std::wstring strState;
		if (iDir > 0)
			strDir = L"_right";
		else
			strDir = L"_left";

		strState = pPlayer->GetCurStateName();

		std::wstring strAnim = m_strCurEquip + strState + strDir;

		if (m_strCurAnim != strAnim)
		{
			m_strCurAnim = strAnim;
			bool bLoop = pPlayer->IsLoop();
			pAnimator->Play(strAnim, bLoop);
		}

		GameObject::LateUpdate();
	}

	void PlayerHat::SetPlayerEquip(const std::wstring& _strEquipName)
	{
		m_strCurEquip = _strEquipName;
		Initialize();
	}
	
}