#include "WDamageFont.h"
#include "WTime.h"
#include "WSceneManger.h"
#include "WEventManager.h"
namespace W
{
	UINT DamageFont::CREATE_ID = 0;

	DamageFont::DamageFont() :
		m_fDeleteTime(1.f),
		m_fCurTime(0.0f)
	{

	}
	DamageFont::~DamageFont()
	{

	}
	void DamageFont::Initialize()
	{
		
	}
	void DamageFont::Update()
	{
		m_fCurTime += Time::DeltaTime();

		m_fAlfhaColor += Time::DeltaTime();

		if (m_fAlfhaColor >= 1.f)
			m_fAlfhaColor = 1.f;


		if (m_fCurTime >= m_fDeleteTime)
		{
			m_bActive = false;
			m_fCurTime = 0.f;
			
			EventManager::EraseObject(this);
			ObjectPoolManager::AddObjectPool(GetName(), this);
			return;
		}

		GameObject::Update();

	}
	void DamageFont::LateUpdate()
	{
		Transform* pTr = GetComponent<Transform>();

		Vector3 vPosition = pTr->GetPosition();
		vPosition.y += (0.4f * Time::DeltaTime());
		pTr->SetPosition(vPosition);

		GameObject::LateUpdate();
	}
	void DamageFont::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		Protocol::S_STATE pkt;

		UCHAR cLayer = (UCHAR)GetLayerType();
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		pkt.set_state_value((m_iDamage <<16) | (UINT)(m_fAlfhaColor * 10000));
		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneID()));
	}
	void DamageFont::CheckDamage(UINT _iDamage)
	{
		m_iDamage = _iDamage;
		m_fAlfhaColor = 0.2f;
		m_fCurTime = 0.0f;
	}
}