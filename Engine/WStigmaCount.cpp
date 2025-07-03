#include "WStigmaCount.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WSceneManger.h"
namespace W
{
	UINT StigmaCount::CREATE_ID = 0;

	StigmaCount::StigmaCount():
		m_iPrevNumber(0),
		m_iNumber(0),
		m_pOwner(nullptr)
	{
		GetComponent<Transform>()->SetScale(0.14f * 1.4f, 0.16f * 1.4f, 0.f);
	}
	StigmaCount::~StigmaCount()
	{

	}
	void StigmaCount::Initialize()
	{
		
	}
	void StigmaCount::Update()
	{
		if (m_iNumber == 0)
			return;

		Vector3 vPosition =
			m_pOwner->GetComponent<Transform>()->GetPosition();
		vPosition.z = -2.2f;
		vPosition.x -= 0.04f;
		vPosition.y -= 0.02f;

		GetComponent<Transform>()->SetPosition(vPosition);

		GameObject::Update();
	}
	void StigmaCount::LateUpdate()
	{
		if (m_iNumber == 0)
			return;
		GameObject::LateUpdate();
	}

	void StigmaCount::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		if (m_iPrevNumber == m_iNumber)
			return;

		m_iPrevNumber = m_iNumber;
		Protocol::S_STATE pkt;

		UCHAR cLayer = (UCHAR)GetLayerType();
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		pkt.set_state_value(m_iNumber);
	
		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneName()));
	}
	
	void StigmaCount::set_count(UINT _iCount)
	{
		if (_iCount>7)
			return;

		m_iNumber = _iCount;
	
	}
}