#include "WInputObject.h"
#include "WAnimator.h"
#include "WInputBackground.h"
#include "WInput.h"
#include "WPlayer.h"
namespace W
{
	UINT InputObject::CREATE_ID = 0;

	InputObject::InputObject(eKeyCode _eKeyCode):
		m_eKeyCode(_eKeyCode),
		m_pOwner(nullptr),
		m_bRender(true),
		m_bActive(true)
	{
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"clear", Vector2(0.0f, 0.0f), Vector2(80.f, 79.0f), 7, Vector2(100.f, 100.f), Vector2::Zero, 0.15f);
		pAnim->Create(L"failed", Vector2(0.0f, 79.0f), Vector2(89.f, 77.0f), 4, Vector2(100.f, 100.f), Vector2::Zero, 0.15f);

		pAnim->CompleteEvent(L"failed") = std::bind(&InputObject::failed, this);
		pAnim->CompleteEvent(L"clear") = std::bind(&InputObject::clear, this);

		pAnim->Play(L"clear", true);
		pAnim->Stop(true);
	}

	InputObject::~InputObject()
	{

	}

	void InputObject::Initialize()
	{

	}

	void InputObject::Update()
	{
		if (!m_bRender)
			return;

		if (m_bActive)
			check();

		GameObject::Update();
	}

	void InputObject::LateUpdate()
	{
		if (!m_bRender)
			return;

		GameObject::LateUpdate();
	}

	void InputObject::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		Animator* pAnimator = GetComponent<Animator>();
		if (!pAnimator || pAnimator->TrySendPacket())
			return;

		Protocol::S_STATE pkt;

		UCHAR cLayer = (UCHAR)eLayerType::Object;
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cAnimIdx = pAnim->GetCurIndex();

		pkt.set_state_value(cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, GetExclusiveClients());
	}

	void InputObject::SetClear(bool _bClear)
	{
		std::wstring strAnim = L"";
		strAnim = _bClear == true ? L"clear" : L"failed";
		GetComponent<Animator>()->Stop(false);
		GetComponent<Animator>()->Play(strAnim, true);
	}

	void InputObject::SetTarget(Player* _pPlayer)
	{
		m_pTarget = _pPlayer;
		AddExclusiveClient(_pPlayer->GetPlayerID());
	}

	void InputObject::check()
	{
		if (m_pOwner->GetCurIndex() != m_iIndex)
		{
			GameObject::Update();
			return;
		}
		bool m_bInput = false;
		bool m_bClear = false;

		if (Input::GetKeyDown(m_pTarget->GetPlayerID(), eKeyCode::RIGHT))
		{
			m_bInput = true;

			if (m_eKeyCode == eKeyCode::RIGHT)
				m_bClear = true;
		}

		else if (Input::GetKeyDown(m_pTarget->GetPlayerID(), eKeyCode::LEFT))
		{
			m_bInput = true;

			if (m_eKeyCode == eKeyCode::LEFT)
				m_bClear = true;
		}

		else if (Input::GetKeyDown(m_pTarget->GetPlayerID(), eKeyCode::UP))
		{
			m_bInput = true;

			if (m_eKeyCode == eKeyCode::UP)
				m_bClear = true;
		}

		else if (Input::GetKeyDown(m_pTarget->GetPlayerID(), eKeyCode::DOWN))
		{
			m_bInput = true;

			if (m_eKeyCode == eKeyCode::DOWN)
				m_bClear = true;
		}

		//´­·¶´Ù¸é
		if (m_bInput)
		{
			if (m_bClear)
				m_pOwner->Next();

			SetClear(m_bClear);

			m_bActive = false;
		}

	}

	void InputObject::failed()
	{
		m_pOwner->Failed();
	}

	void InputObject::clear()
	{
		m_bRender = false;
	}
}