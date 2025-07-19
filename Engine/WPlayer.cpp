#include "WPlayer.h"
#include "WRigidbody.h"
#include "WCollider2D.h"
#include "WPlayerScript.h"
#include "WResources.h"
#include "WInput.h"
#include "WAnimator.h"
#include "WPlayerBody.h"
#include "WPlayerArm.h"
#include "WPlayerHead.h"
#include "WEyes.h"
#include "WPlayerHair.h"

#include "WTime.h"
#include "WShadow.h"
#include "ObjectState.pb.h"
#include "WSceneManger.h"


namespace W
{
	UINT Player::CREATE_ID = 0;

	Player::Player():
		m_pShadow(nullptr),
		m_bAnimLoop(true),
		m_bAlert(false),
		m_bActiveDark(false),
		m_bActiveShadow(false),
		m_iDir(1),
		m_bAlertTime(2.f),
		m_fChangeTime(0.25f),
		m_vecChildObj{},
		m_ePlayerState(ePlayerState::stand),
		m_strCurStateName(L"_jump")
	{
		SetName(L"Player");

		Collider2D* pCollider = AddComponent<Collider2D>();
		AddComponent<Rigidbody>()->SetGround(false);

		GetComponent<Transform>()->SetScale(1.5f, 1.5f, 0.f);
		GetComponent<Transform>()->SetPosition(0.f, -5.f, -2.f);

		pCollider->SetSize(Vector2(0.5f, 0.5f));

		m_vecChildObj.resize(ePlayerPart::End);

		PlayerBody* pPlayerBody = new PlayerBody();
		pPlayerBody->SetPlayer(this);
		pPlayerBody->Initialize();
		m_vecChildObj[ePlayerPart::Body] = pPlayerBody;

		PlayerHead* pPlayerHead = new PlayerHead();
		pPlayerHead->SetPlayer(this);
		pPlayerHead->Initialize();
		m_vecChildObj[ePlayerPart::Head] = pPlayerHead;

		PlayerArm* pPlayerArm = new PlayerArm();
		pPlayerArm->SetPlayer(this);
		pPlayerArm->Initialize();
		m_vecChildObj[ePlayerPart::Arm] = pPlayerArm;

		m_pShadow = new Shadow();

		AddComponent<PlayerScript>()->Initialize();
	}
	Player::~Player()
	{
		for (GameObject* pChildObj : m_vecChildObj)
		{
			delete pChildObj;
			pChildObj = nullptr;
		}

		if (m_pShadow)
		{
			delete m_pShadow;
			m_pShadow = nullptr;
		}
	}
	void Player::Initialize()
	{
		SetState(eState::Active);
		GetScript<PlayerScript>()->RegisterSkill();

		m_strCurStateName = L"_jump";

		GetComponent<Transform>()->SetPosition(Vector3::Zero);
		GetComponent<Rigidbody>()->SetGround(false);
	}

	void Player::Update()
	{
		if (m_bAlert)
		{
			m_bAlertTime -= Time::DeltaTime();

			if (m_bAlertTime <= 0.f)
			{
				SetAlert(false);
			}
		}
		

		GameObject::Update();

		if (m_pShadow)
			m_pShadow->Update();

		child_update();
	}
	void Player::LateUpdate()
	{
		GameObject::LateUpdate();

		if (m_pShadow)
			m_pShadow->LateUpdate();

		child_lateupdate();

	}

	void Player::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();
		update_state();
	}

	UINT Player::GetPlayerEquipID(eEquipType _eEquipType)
	{
		switch (_eEquipType)
		{
		case eEquipType::Hat:
			return GetPlayerChild<PlayerHead>(ePlayerPart::Head)->GetHatEquipID();
		case eEquipType::Top:
			return GetPlayerChild<PlayerBody>(ePlayerPart::Body)->GetTopEquipID();
		case eEquipType::Bottom:
			return GetPlayerChild<PlayerBody>(ePlayerPart::Body)->GetBottomEquipID();
		case eEquipType::Shoes:
			return GetPlayerChild<PlayerBody>(ePlayerPart::Body)->GetShoesEquipID();
		case eEquipType::Weapon:
			return GetPlayerChild<PlayerArm>(ePlayerPart::Arm)->GetWeaponEquipID();
		}
		return {};
	}


	void Player::ActiveShadow()
	{
		m_bActiveShadow = true;
	}

	void Player::InactiveShadow()
	{
		m_bActiveShadow = false;
	}

	void Player::SetAlert(bool _bAlert)
	{
		static_cast<PlayerHead*>(m_vecChildObj[ePlayerPart::Head])->SetAlert(_bAlert);
		m_bAlert = _bAlert;

		if (m_bAlert)
			m_bAlertTime = 2.f;
	}

	void Player::SetEquip(eEquipType _eType, UINT _iItemID)
	{
		auto wpItem = ItemManager::GetItemInfo(_iItemID);
		if (auto spItem = wpItem.lock())
		{
			const wstring& strItemName = StringToWString(spItem->strItemName);
			switch (_eType)
			{
			case eEquipType::Hat:
				GetPlayerChild<PlayerHead>(ePlayerPart::Head)->SetEquipHat(strItemName);
				break;
			case eEquipType::Top:
				GetPlayerChild<PlayerBody>(ePlayerPart::Body)->SetEquipTop(strItemName);
				break;
			case eEquipType::Bottom:
				GetPlayerChild<PlayerBody>(ePlayerPart::Body)->SetEquipBottom(strItemName);
				break;
			case eEquipType::Shoes:
				GetPlayerChild<PlayerBody>(ePlayerPart::Body)->SetEquipShoes(strItemName);
				break;
			case eEquipType::Weapon:
				GetPlayerChild<PlayerArm>(ePlayerPart::Arm)->SetEquipWeapon(strItemName);
				break;
			}

			Reset_Animation();
		}
	}

	void Player::DisableEquip(eEquipType _eType)
	{
		switch (_eType)
		{
		case eEquipType::Hat:
			GetPlayerChild<PlayerHead>(ePlayerPart::Head)->SetEquipHat(L"");
			break;
		case eEquipType::Top:
			GetPlayerChild<PlayerBody>(ePlayerPart::Body)->SetEquipTop(L"");
			break;
		case eEquipType::Bottom:
			GetPlayerChild<PlayerBody>(ePlayerPart::Body)->SetEquipBottom(L"");
			break;
		case eEquipType::Shoes:
			GetPlayerChild<PlayerBody>(ePlayerPart::Body)->SetEquipShoes(L"");
			break;
		case eEquipType::Weapon:
			GetPlayerChild<PlayerArm>(ePlayerPart::Arm)->SetEquipWeapon(L"");
			break;
		}

		Reset_Animation();
	}

	UINT64 Player::GetPlayerEquips()
	{
		UINT iHairID = GetPlayerChild<PlayerHead>(ePlayerPart::Head)->GetHairID();
		UINT iEyeID = GetPlayerChild<PlayerHead>(ePlayerPart::Head)->GetEyeID();
		UINT iHatID = GetPlayerChild<PlayerHead>(ePlayerPart::Head)->GetHatEquipID();
		UINT iTopID = GetPlayerChild<PlayerBody>(ePlayerPart::Body)->GetTopEquipID();
		UINT iBottomID = GetPlayerChild<PlayerBody>(ePlayerPart::Body)->GetBottomEquipID();
		UINT iShoesID = GetPlayerChild<PlayerBody>(ePlayerPart::Body)->GetShoesEquipID();
		UINT iWeaponID = GetPlayerChild<PlayerArm>(ePlayerPart::Arm)->GetWeaponEquipID();

		UINT64 iEquipIDs =
			(UINT64)iHairID | ((UINT64)iEyeID << 8) | ((UINT64)iHatID << 16)
			| ((UINT64)iTopID << 24) | ((UINT64)iBottomID << 32) | ((UINT64)iShoesID << 40)
			| ((UINT64)iWeaponID << 48);

		return iEquipIDs;
	}

	void Player::SetAnimStop(bool _bStop)
	{
		m_bAnimStop = _bStop;

		GetPlayerChild<PlayerBody>(ePlayerPart::Body)->SetStop(_bStop);
		GetPlayerChild<PlayerBody>(ePlayerPart::Head)->SetStop(_bStop);
		GetPlayerChild<PlayerBody>(ePlayerPart::Arm)->SetStop(_bStop);
	}

	void Player::child_update()
	{
		for (GameObject* pObj : m_vecChildObj)
		{
			pObj->Update();
		}
	}
	
	void Player::child_lateupdate()
	{
		for (GameObject* pObj : m_vecChildObj)
		{
			pObj->LateUpdate();
		}
	}

	void Player::update_state()
	{
		Animator* pAnim = m_vecChildObj[0]->GetComponent<Animator>();
		if (!pAnim->TrySendPacket())
			return;
		
		Protocol::S_STATE pkt;
		PlayerScript* pScript = GetScript<PlayerScript>();
		
		if (pScript->IsSkillOn())
			pkt.set_state(WstringToString(pScript->GetCurSkillName()));
		else
			pkt.set_state(WstringToString(m_strCurStateName));

		UCHAR cLayer = (UCHAR)eLayerType::Player;
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		UCHAR cDir = m_iDir > 0 ? 1 : 0; 
		UCHAR cAnimIdx = pAnim->GetActiveAnimation()->GetCurIndex();
		UCHAR cAlert = m_bAlert ? 1 : 0;
		UCHAR cShadow = m_bActiveShadow ? 1 : 0;
		UCHAR cDark = m_bActiveDark ? 1 : 0;

		//여기 IsDark() 하나 더 보내야함
		pkt.set_state_value((cDark << 28) | (cShadow<<24) | (cAlert<<16) | (cDir << 8) | cAnimIdx);

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneID()));
	}
	void Player::update_shadow()
	{
		
	}
	void Player::Reset_Animation()
	{
		GetPlayerChild<PlayerBody>(ePlayerPart::Body)->SetAnimationIndex();
		GetPlayerChild<PlayerBody>(ePlayerPart::Head)->SetAnimationIndex();
		GetPlayerChild<PlayerBody>(ePlayerPart::Arm)->SetAnimationIndex();
	}

	void Player::SetHair(UINT _iHairNum)
	{
		static_cast<PlayerHead*>(m_vecChildObj[ePlayerPart::Head])->SetHair(_iHairNum);
		Reset_Animation();
	}
	void Player::SetEye(UINT _iEyeNum)
	{
		static_cast<PlayerHead*>(m_vecChildObj[ePlayerPart::Head])->SetEye(_iEyeNum);
		Reset_Animation();
	}
}