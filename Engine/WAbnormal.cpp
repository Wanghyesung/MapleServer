#include "WAbnormal.h"
#include "WTime.h"
#include "WBattleManager.h"
#include "WPlayer.h"
#include "WEventManager.h"
#include "WAnimator.h"
#include "WSceneManger.h"
namespace W
{
	UINT Abnormal::CREATE_ID = 0;

	Abnormal::Abnormal() :
		m_pTarget(nullptr),
		m_bRender(false),
		m_bRenderPosUp(true),
		m_fDeleteTime(0.f),
		m_fCurTime(0.f)
	{

	}
	Abnormal::~Abnormal()
	{

	}
	void Abnormal::Initialize()
	{
	}
	void Abnormal::Update()
	{	
		GameObject::Update();
	}
	void Abnormal::LateUpdate()
	{	
		m_fCurTime += Time::DeltaTime();

		if (m_fCurTime >= m_fDeleteTime ||
			m_pTarget->GetState() == GameObject::Dead)
		{
			EventManager::DeleteObject(this);
			Restore();
			return;
		}

		Vector3 vTargetPos = m_pTarget->GetComponent<Transform>()->GetPosition();
		vTargetPos.z -= 0.5f;
		if(m_bRenderPosUp)
			vTargetPos.y += 0.5f;
		else
			vTargetPos.y -= 0.2f;

		GetComponent<Transform>()->SetPosition(vTargetPos);

		GameObject::LateUpdate();
	}
	void Abnormal::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();
		
		update_packet();
	}
	void Abnormal::Restore()
	{

	}
	void Abnormal::SetTarget(Player* _pTarget)
	{
		m_pTarget = _pTarget;
		SetSceneID(_pTarget->GetSceneID());
	}

	void Abnormal::update_packet()
	{
		Animator* pAnimator = GetComponent<Animator>();
		if (!pAnimator || pAnimator->TrySendPacket())
			return;

		Protocol::S_STATE pkt;

		UCHAR cLayer = (UCHAR)GetLayerType();
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cDir = 0;
		UCHAR cAnimIdx = pAnim->GetCurIndex();

		pkt.set_state_value((cDir << 8) | cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneID()));
	}
	

}