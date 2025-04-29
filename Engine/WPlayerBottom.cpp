#include "WPlayerBottom.h"
#include "WAnimator.h"
#include "WResources.h"
#include "WPlayerBody.h"

namespace W
{
	PlayerBottom::PlayerBottom():
		m_pPlayerBody(nullptr),
		m_strCurAnim{},
		m_strCurEquip{}
	{
		
		Animator* pAnimator = AddComponent<Animator>();
	}
	PlayerBottom::~PlayerBottom()
	{
		
	}
	void PlayerBottom::Initialize()
	{
		if (m_strCurEquip.size() == 0)
			return;

		Animator* pAnimator = GetComponent<Animator>();
	
		pAnimator->Create(m_strCurEquip + L"_alert_left", Vector2(0.0f, 0.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.18f);
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
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_left")->Create(m_strCurEquip +L"_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_left")->Create(m_strCurEquip +L"_swingQS_left", Vector2(450.0f, 1200.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.14f);


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


		Vector3 vScale = m_pPlayerBody->GetComponent<Transform>()->GetScale();
		GetComponent<Transform>()->SetScale(vScale);
	}

	void PlayerBottom::Update()
	{
		if (m_strCurEquip.size() == 0)
			return;

		GameObject::Update();
	}
	void PlayerBottom::LateUpdate()
	{
		if (m_strCurEquip.size() == 0)
			return;

		Animator* pAnimator = GetComponent<Animator>();
		Vector3 vPlayerPos = m_pPlayerBody->GetComponent<Transform>()->GetPosition();
		vPlayerPos.z -= 0.01f;
		GetComponent<Transform>()->SetPosition(vPlayerPos);

		Player* pPlayer = m_pPlayerBody->GetPlayer();
		int iDir = pPlayer->GetDir();
		std::wstring strDir;
		std::wstring strState;
		strDir = iDir > 0 ? L"_right" : L"_left";

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

	void PlayerBottom::SetPlayerEquip(const std::wstring& _strEquipName)
	{
		m_strCurEquip = _strEquipName;
		Initialize();
	}
	
}