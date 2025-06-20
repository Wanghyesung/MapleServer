#include "WPlayerHair.h"
#include "WAnimator.h"
#include "WResources.h"
#include "WPlayerHead.h"

namespace W
{
	UINT PlayerHair::CREATE_ID = 0;

	PlayerHair::PlayerHair():
		m_bHairDown(true),
		m_iHairNum(0)
	{
		Animator* pAnimator = AddComponent<Animator>();
	}
	PlayerHair::~PlayerHair()
	{
	}
	void PlayerHair::Initialize()
	{
		Animator* pAnimator = GetComponent<Animator>();

		std::wstring strNum = std::to_wstring(m_iHairNum);

		pAnimator->Create(L"hair_alert_left" + strNum, Vector2(0.0f, 0.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_jump_left" + strNum, Vector2(0.0f, 150.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_ladder_left" + strNum, Vector2(0.0f, 300.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_pronestab_left" + strNum, Vector2(0.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_proneattack_left" + strNum, Vector2(150.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_stab_left" + strNum, Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_stand_left" + strNum, Vector2(0.0f, 750.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_swing1_left" + strNum, Vector2(0.0f, 900.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_swing2_left" + strNum, Vector2(0.0f, 1050.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_swingOF_left" + strNum, Vector2(0.0f, 1200.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_walk_left" + strNum, Vector2(0.0f, 1350.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_swingQS_left" + strNum, Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"hair_swingQS_left" + strNum)->Create(L"hair_swingQS_left" + strNum, Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"hair_swingQS_left" + strNum)->Create(L"hair_swingQS_left" + strNum, Vector2(450.0f, 1200.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);


		pAnimator->Create(L"hair_alert_right" + strNum, Vector2(450.0f, 0.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_jump_right" + strNum, Vector2(450.0f, 150.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_ladder_right" + strNum, Vector2(450.0f, 300.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_pronestab_right" + strNum, Vector2(450.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_proneattack_right" + strNum, Vector2(300.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_stab_right" + strNum, Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_stand_right" + strNum, Vector2(450.0f, 750.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_swing1_right" + strNum, Vector2(450.0f, 900.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_swing2_right" + strNum, Vector2(450.0f, 1050.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_swingOF_right" + strNum, Vector2(450.0f, 1200.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_walk_right" + strNum, Vector2(450.0f, 1350.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hair_swingQS_right" + strNum, Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"hair_swingQS_right" + strNum)->Create(L"hair_swingQS_right" + strNum, Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"hair_swingQS_right" + strNum)->Create(L"hair_swingQS_right" + strNum, Vector2(0, 1200.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);

		//pAtlasBdoy = Resources::Load<Texture>(L"PlayerHairDownTex" + strNum, L"..\\Resources\\Texture\\Player\\hair\\PlayerHairDown" + strNum + L".png");
		pAnimator->Create(L"hairdown_alert_left" + strNum, Vector2(0.0f, 0.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_jump_left" + strNum, Vector2(0.0f, 150.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_ladder_left" + strNum, Vector2(0.0f, 300.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_pronestab_left" + strNum, Vector2(0.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_proneattack_left" + strNum, Vector2(150.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_stab_left" + strNum, Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_stand_left" + strNum, Vector2(0.0f, 750.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_swing1_left" + strNum, Vector2(0.0f, 900.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_swing2_left" + strNum, Vector2(0.0f, 1050.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_swingOF_left" + strNum, Vector2(0.0f, 1200.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_walk_left" + strNum, Vector2(0.0f, 1350.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_swingQS_left" + strNum, Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"hairdown_swingQS_left" + strNum)->Create(L"hairdown_swingQS_left" + strNum, Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"hairdown_swingQS_left" + strNum)->Create(L"hairdown_swingQS_left" + strNum, Vector2(450.0f, 1200.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);


		pAnimator->Create(L"hairdown_alert_right" + strNum, Vector2(450.0f, 0.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_jump_right" + strNum, Vector2(450.0f, 150.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_ladder_right" + strNum, Vector2(450.0f, 300.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_pronestab_right" + strNum, Vector2(450.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_proneattack_right" + strNum, Vector2(300.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_stab_right" + strNum, Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_stand_right" + strNum, Vector2(450.0f, 750.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_swing1_right" + strNum, Vector2(450.0f, 900.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_swing2_right" + strNum, Vector2(450.0f, 1050.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_swingOF_right" + strNum, Vector2(450.0f, 1200.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_walk_right" + strNum, Vector2(450.0f, 1350.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"hairdown_swingQS_right" + strNum, Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->FindAnimation(L"hairdown_swingQS_right" + strNum)->Create(L"hairdown_swingQS_right" + strNum, Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"hairdown_swingQS_right" + strNum)->Create(L"hairdown_swingQS_right" + strNum, Vector2(0, 1200.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);

		Vector3 vScale = m_pPlayerHead->GetComponent<Transform>()->GetScale();
		GetComponent<Transform>()->SetScale(vScale);
	}
	void PlayerHair::Update()
	{
		GameObject::Update();
	}
	void PlayerHair::LateUpdate()
	{
		Animator* pAnimator = GetComponent<Animator>();
		Vector3 vPlayerPos = m_pPlayerHead->GetComponent<Transform>()->GetPosition();
		GetComponent<Transform>()->SetPosition(vPlayerPos);

		Player* pPlayer = m_pPlayerHead->GetPlayer();

		int iDir = pPlayer->GetDir();
		std::wstring strHair = L"hair";
		std::wstring strDir;
		std::wstring strState;
		strDir = iDir > 0 ? L"_right" : L"_left";

		strState = pPlayer->GetCurStateName();

		if (m_bHairDown)
			strHair += L"down";

		std::wstring strNum = std::to_wstring(m_iHairNum);

		std::wstring strAnim = strHair + strState + strDir + strNum;

		if (m_strCurAnim != strAnim)
		{
			m_strCurAnim = strAnim;
			bool bLoop = pPlayer->IsLoop();
			pAnimator->Play(strAnim, bLoop);
		}

		GameObject::LateUpdate();
	}
	
	void PlayerHair::SetHair(UINT _iNum)
	{
		m_iHairNum = _iNum;
		Initialize();
	}
}