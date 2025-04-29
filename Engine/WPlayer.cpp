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
#include "NetFunc.h"
#include "ClientPacketHandler.h"
#include "Room.h"

extern Room GRoom;

namespace W
{

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
		PlayerScript* pPlayerScript = AddComponent<PlayerScript>();
		Collider2D* pCollider = AddComponent<Collider2D>();
		AddComponent<Rigidbody>()->SetGround(false);

		GetComponent<Transform>()->SetScale(1.5f, 1.5f, 0.f);
		GetComponent<Transform>()->SetPosition(0.f, -5.f, -2.f);

		pCollider->SetSize(Vector2(0.5f, 0.5f));

		m_vecChildObj.resize(3);


		PlayerBody* pPlayerBody = new PlayerBody();
		pPlayerBody->SetPlayer(this);
		pPlayerBody->Initialize();
		pPlayerBody->SetEquipBottom(L"10_bottom");
		pPlayerBody->SetEquipTop(L"10_top");
		pPlayerBody->SetEquipShoes(L"10_shoes");
		m_vecChildObj[0] = pPlayerBody;

		PlayerHead* pPlayerHead = new PlayerHead();
		pPlayerHead->SetPlayer(this);
		pPlayerHead->Initialize();
		pPlayerHead->SetEquipHat(L"10_hat");
		m_vecChildObj[1] = pPlayerHead;

		PlayerArm* pPlayerArm = new PlayerArm();
		pPlayerArm->SetPlayer(this);
		pPlayerArm->Initialize();
		pPlayerArm->SetEquipWeapon(L"10_weapon");
		m_vecChildObj[2] = pPlayerArm;

		pPlayerScript->Initialize();

		m_pShadow = new Shadow();
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

		UpdatePacket();
	}

	void Player::UpdatePacket()
	{
		GameObject::UpdatePacket();

		update_state();
	}

	void Player::SetAlert(bool _bAlert)
	{
		dynamic_cast<PlayerHead*>(m_vecChildObj[1])->SetAlert(_bAlert);
		m_bAlert = _bAlert;

		if (m_bAlert)
			m_bAlertTime = 2.f;
	}

	void Player::SetEquip(eEquipType _eType, const std::wstring _strEquipName)
	{
		switch (_eType)
		{
		case eEquipType::Hat:
			GetPlayerChild<PlayerHead>()->SetEquipHat(_strEquipName);
			break;
		case eEquipType::Top:
			GetPlayerChild<PlayerBody>()->SetEquipTop(_strEquipName);
			break;
		case eEquipType::Bottom:
			GetPlayerChild<PlayerBody>()->SetEquipBottom(_strEquipName);
			break;
		case eEquipType::Shoes:
			GetPlayerChild<PlayerBody>()->SetEquipShoes(_strEquipName);
			break;
		case eEquipType::Weapon:
			GetPlayerChild<PlayerArm>()->SetEquipWeapon(_strEquipName);
			break;
		}

		Reset_Animation();
	}

	void Player::DisableEquip(eEquipType _eType, const std::wstring _strEquipName)
	{
		switch (_eType)
		{
		case eEquipType::Hat:
			GetPlayerChild<PlayerHead>()->SetEquipHat(L"");
			break;
		case eEquipType::Top:
			GetPlayerChild<PlayerBody>()->SetEquipTop(L"");
			break;
		case eEquipType::Bottom:
			GetPlayerChild<PlayerBody>()->SetEquipBottom(L"");
			break;
		case eEquipType::Shoes:
			GetPlayerChild<PlayerBody>()->SetEquipShoes(L"");
			break;
		case eEquipType::Weapon:
			GetPlayerChild<PlayerArm>()->SetEquipWeapon(L"");
			break;
		}

		Reset_Animation();
	}

	void Player::SetAnimStop(bool _bStop)
	{
		m_bAnimStop = _bStop;

		dynamic_cast<PlayerBody*>(m_vecChildObj[0])->SetStop(_bStop);
		dynamic_cast<PlayerHead*>(m_vecChildObj[1])->SetStop(_bStop);
		dynamic_cast<PlayerArm*>(m_vecChildObj[2])->SetStop(_bStop);
	}

	void Player::child_update()
	{
		for (GameObject* pObj : m_vecChildObj)
		{
			pObj->Update();
		}
	}
	//void Player::child_render()
	//{
	//	for (GameObject* pObj : m_vecChildObj)
	//	{
	//		pObj->Render();
	//	}
	//}
	void Player::child_lateupdate()
	{
		for (GameObject* pObj : m_vecChildObj)
		{
			pObj->LateUpdate();
		}
	}

	void Player::update_state()
	{
		Protocol::S_STATE pkt;
		pkt.set_state(WstringToString(m_strCurStateName));

		UCHAR cLayer = (UCHAR)eLayerType::Player;
		UINT iObjectID = m_iPlayerID;
		pkt.set_layer_id((cLayer << 24) | m_iPlayerID);

		UCHAR cDir = m_iDir > 0 ? 1 : 0; //0보다 크면 오른쪽 
		UCHAR cAnimIdx = m_vecChildObj[0]->GetComponent<Animator>()->GetActiveAnimation()->GetCurIndex();
		
		pkt.set_anim((cDir << 8) | cAnimIdx);


		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Broadcast(pSendBuffer);

	}
	void Player::Reset_Animation()
	{
		dynamic_cast<PlayerBody*>(m_vecChildObj[0])->SetAnimationIndex();
		dynamic_cast<PlayerHead*>(m_vecChildObj[1])->SetAnimationIndex();
		dynamic_cast<PlayerArm*>(m_vecChildObj[2])->SetAnimationIndex();
	}

	void Player::SetHair(UINT _iHairNum)
	{
		dynamic_cast<PlayerHead*>(m_vecChildObj[1])->SetHair(_iHairNum);
		Reset_Animation();
	}
	void Player::SetEye(UINT _iEyeNum)
	{
		dynamic_cast<PlayerHead*>(m_vecChildObj[1])->SetEye(_iEyeNum);
		Reset_Animation();
	}
}