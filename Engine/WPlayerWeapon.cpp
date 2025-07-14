#include "WPlayerWeapon.h"
#include "WAnimator.h"
#include "WResources.h"
#include "WPlayerArm.h"

namespace W
{
	UINT PlayerWeapon::CREATE_ID = 0;

	PlayerWeapon::PlayerWeapon():
		m_pPlayerArm(nullptr),
		m_strCurEquip(L"10_weapon"),
		m_strCurAnim{}
	{
		
		Animator* pAnimator = AddComponent<Animator>();
	}
	PlayerWeapon::~PlayerWeapon()
	{

	}
	void PlayerWeapon::Initialize()
	{
		if (m_strCurEquip.size() == 0)
			return;

		Animator* pAnimator = GetComponent<Animator>();

		pAnimator->Create(m_strCurEquip + L"_alert_left", Vector2(0.0f, 0.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_jump_left", Vector2(0.0f, 150.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_ladder_left", Vector2(0.0f, 300.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_pronestab_left", Vector2(0.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_proneattack_left", Vector2(150.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_stab_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_stand_left", Vector2(0.0f, 750.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_swing1_left", Vector2(0.0f, 900.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_swing2_left", Vector2(0.0f, 1050.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_swingOF_left", Vector2(0.0f, 1200.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_walk_left", Vector2(0.0f, 1350.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_left")->Create(m_strCurEquip + L"_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_left")->Create(m_strCurEquip + L"_swingQS_left", Vector2(450.0f, 1200.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);


		pAnimator->Create(m_strCurEquip + L"_alert_right", Vector2(450.0f, 0.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_jump_right", Vector2(450.0f, 150.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_ladder_right", Vector2(450.0f, 300.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_pronestab_right", Vector2(450.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_proneattack_right", Vector2(300.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_stab_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_stand_right", Vector2(450.0f, 750.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_swing1_right", Vector2(450.0f, 900.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_swing2_right", Vector2(450.0f, 1050.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_swingOF_right", Vector2(450.0f, 1200.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_walk_right", Vector2(450.0f, 1350.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(m_strCurEquip + L"_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_right")->Create(m_strCurEquip + L"_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(m_strCurEquip + L"_swingQS_right")->Create(m_strCurEquip + L"_swingQS_right", Vector2(0, 1200.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);


		Vector3 vScale = m_pPlayerArm->GetComponent<Transform>()->GetScale();
		GetComponent<Transform>()->SetScale(vScale);

	}
	void PlayerWeapon::Update()
	{
		if (m_strCurEquip.size() == 0)
			return;

		GameObject::Update();
	}
	void PlayerWeapon::LateUpdate()
	{
		
	}

	void PlayerWeapon::SetPlayerEquip(const std::wstring& _strEquipName)
	{
		auto wpItem = ItemManager::GetItemInfo(_strEquipName);
		if (auto spItem = wpItem.lock())
		{
			m_strCurEquip = StringToWString(spItem->strItemName);
			m_iItemID = spItem->iItemID;
			Initialize();
		}
	}

}