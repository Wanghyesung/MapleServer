#include "WPlayerHead.h"
#include "WAnimator.h"
#include "WResources.h"
#include "WPlayerHat.h"
#include "WEyes.h"
#include "WPlayerHair.h"
#include "WItemManager.h"
namespace W
{
	UINT PlayerHead::CREATE_ID = 0;

	PlayerHead::PlayerHead():
		m_pPlayerHat(nullptr),
		m_pPlayerHair(nullptr),
		m_pPlayerEyes(nullptr)
	{
	
	}
	PlayerHead::~PlayerHead()
	{
		if (m_pPlayerHat)
		{
			delete m_pPlayerHat;
			m_pPlayerHat = nullptr;
		}
		if (m_pPlayerHair)
		{
			delete m_pPlayerHair;
			m_pPlayerHair = nullptr;
		}
		if (m_pPlayerEyes)
		{
			delete m_pPlayerEyes;
			m_pPlayerEyes = nullptr;
		}
	}
	void PlayerHead::Initialize()
	{
		Animator* pAnimator = AddComponent<Animator>();

		pAnimator->Create(L"head_alert_left", Vector2(0.0f, 0.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_jump_left", Vector2(0.0f, 150.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_ladder_left", Vector2(0.0f, 300.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_pronestab_left", Vector2(0.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_proneattack_left", Vector2(150.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_stab_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_stand_left", Vector2(0.0f, 750.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_swing1_left", Vector2(0.0f, 900.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_swing2_left", Vector2(0.0f, 1050.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_swingOF_left", Vector2(0.0f, 1200.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_walk_left", Vector2(0.0f, 1350.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"head_swingQS_left")->Create(L"head_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"head_swingQS_left")->Create(L"head_swingQS_left", Vector2(450.0f, 1200.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);

		pAnimator->Create(L"head_alert_right", Vector2(450.0f, 0.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_jump_right", Vector2(450.0f, 150.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_ladder_right", Vector2(450.0f, 300.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_pronestab_right", Vector2(450.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_proneattack_right", Vector2(300.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);

		pAnimator->Create(L"head_stab_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_stand_right", Vector2(450.0f, 750.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_swing1_right", Vector2(450.0f, 900.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_swing2_right", Vector2(450.0f, 1050.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_swingOF_right", Vector2(450.0f, 1200.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_walk_right", Vector2(450.0f, 1350.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"head_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"head_swingQS_right")->Create(L"head_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"head_swingQS_right")->Create(L"head_swingQS_right", Vector2(0, 1200.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);

		Vector3 vScale = m_pPlayer->GetComponent<Transform>()->GetScale();
		GetComponent<Transform>()->SetScale(vScale);

		m_pPlayerHat = new PlayerHat();
		m_pPlayerHat->SetPlayerHead(this);

		m_pPlayerEyes = new Eyes();
		m_pPlayerEyes->SetPlayerHead(this);
		m_pPlayerEyes->Initialize();
		
		
		m_pPlayerHair = new PlayerHair();
		m_pPlayerHair->SetPlayerHead(this);
		m_pPlayerHair->Initialize();
	}

	void PlayerHead::Update()
	{
		
	}
	void PlayerHead::LateUpdate()
	{

	}	

	void PlayerHead::SetAlert(bool _bAlert)
	{
		m_pPlayerEyes->SetAlert(_bAlert);
	}

	void PlayerHead::SetEquipHat(const std::wstring& _strEquipName)
	{
		if (_strEquipName.empty())
			m_pPlayerHair->SetHairDown(false);
		else
			m_pPlayerHair->SetHairDown(true);
		
		m_pPlayerHat->SetPlayerEquip(_strEquipName);
	}


	const wstring& PlayerHead::GetHatEquipName()
	{
		return m_pPlayerHat->GetEquipName();
	}


	void PlayerHead::SetAnimationIndex()
	{
		if (m_strCurAnim.size() != 0)
			GetComponent<Animator>()->GetActiveAnimation()->SetIndex(0);
	}
	void PlayerHead::SetStop(bool _bStop)
	{
		GetComponent<Animator>()->Stop(_bStop);
	}

	void PlayerHead::SetHair(UINT _iHairNum)
	{
		m_pPlayerHair->SetHair(_iHairNum);
	}

	void PlayerHead::SetEye(UINT _iEyeNum)
	{
		m_pPlayerEyes->SetEye(_iEyeNum);
	}
	
}