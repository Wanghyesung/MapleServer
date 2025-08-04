#include "WStigmaBack.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WSceneManger.h"
#include "WStigmaCount.h"
#include "WBattleManager.h"
#include "WEventManager.h"
namespace W
{
	UINT StigmaBack::CREATE_ID = 0;
	StigmaBack::StigmaBack():
		m_iCount(0)
	{
		GetComponent<Transform>()->SetScale(1.5f, 1.5f, 0.f);


		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"loop", Vector2(0.0f, 0.0f), Vector2(80.0f, 43.0f), 11, Vector2(80.f, 80.f), Vector2::Zero, 0.07f);

		pAnim->Play(L"loop", true);
	}
	StigmaBack::~StigmaBack()
	{

	}
	void StigmaBack::Initialize()
	{
		m_pCount = new StigmaCount();
		m_pCount->SetSceneID(GetSceneID());
		m_pCount->set_count(0);
		m_pCount->m_pOwner = this;

		EventManager::CreateObject(m_pCount, eLayerType::Object);
	}
	void StigmaBack::Update()
	{
		set_count();

		Vector3 vPosition =	m_pTarget->GetComponent<Transform>()->GetPosition();
		vPosition.y += 0.7f;
		vPosition.z = -2.f;
		GetComponent<Transform>()->SetPosition(vPosition);

		GameObject::Update();
	}
	void StigmaBack::LateUpdate()
	{
		if (m_iCount == 0)
			return;
		GameObject::LateUpdate();
	}

	void StigmaBack::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();
		
		Animator* pAnimator = GetComponent<Animator>();
		if (!pAnimator || pAnimator->TrySendPacket())
			return;

		Protocol::S_STATE pkt;
		UCHAR cLayer = (UCHAR)GetLayerType();
		UINT iObjectID = GetObjectID();
		UINT iSceneID = GetSceneID();
		pkt.set_layer_id((cLayer << 24) | (iSceneID << 16) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cAnimIdx = pAnim->GetCurIndex();

		pkt.set_state_value(cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneID()));
	}
	
	void StigmaBack::DeleteCount()
	{
		EventManager::DeleteObject(m_pCount);
	}

	void StigmaBack::set_count()
	{
		if (m_pTarget == nullptr)
			return;

		UINT iObjID = m_pTarget->GetObjectID();
		UINT iCount = BattleManager::GetStigmaCount(iObjID);
		if (m_iCount != iCount)
		{
			m_iCount = iCount;
			m_pCount->set_count(m_iCount);
		}
	}
}