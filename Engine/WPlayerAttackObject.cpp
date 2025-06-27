#include "WPlayerAttackObject.h"
#include "WAttackScript.h"
#include "WEffect.h"
#include "WSceneManger.h"
#include "WPlayer.h"
#include "WPlayerScript.h"
#include "WEventManager.h"
#include "WAnimator.h"
namespace W
{
	UINT PlayerAttackObject::CREATE_ID = 0;

	PlayerAttackObject::PlayerAttackObject():
		m_iDir(1),
		m_pPlayer(nullptr)
	{

	}
	PlayerAttackObject::~PlayerAttackObject()
	{
		
	}
	void PlayerAttackObject::Initialize()
	{
		AddComponent<AttackScript>()->SetDeleteTime(0.1f);

		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.4f, 0.2f));
	}
	void PlayerAttackObject::Update()
	{
		GameObject::Update();
	}
	void PlayerAttackObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void PlayerAttackObject::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		update_state();
	}

	void PlayerAttackObject::Off()
	{
		GetScript<AttackScript>()->off();

		GetComponent<Collider2D>()->SetActive(false);
	}

	void PlayerAttackObject::PushObjectPool()
	{
		EventManager::AddPlayerPool(this);
	}

	void PlayerAttackObject::update_state()
	{
		Animator* pAnimator = GetComponent<Animator>();
		if (!pAnimator || pAnimator->TrySendPacket())
			return;
		
		Protocol::S_STATE pkt;
		
		UCHAR cLayer = (UCHAR)eLayerType::AttackObject;
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);
		
		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cDir = m_iDir > 0 ? 1 : 0;;
		UCHAR cAnimIdx = pAnim->GetCurIndex();
		
		pkt.set_state_value((cDir << 8) | cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));
		
		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneName()));
	}
	
}