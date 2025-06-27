#include "WMonsterAttackObject.h"
#include "WMonsterAttackScript.h"
#include "WMonster.h"
#include "WMonsterScript.h"
#include "WEffect.h"
#include "WAnimator.h"
#include "WSceneManger.h"
namespace W
{
	UINT MonsterAttackObject::CREATE_ID = 0;

	MonsterAttackObject::MonsterAttackObject() :
		m_pOwner(nullptr),
		m_iDir(1)
	{
		
		AddComponent<MonsterAttackScript>();
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetActive(false);
	}

	MonsterAttackObject::~MonsterAttackObject()
	{

	}

	void MonsterAttackObject::Initialize()
	{

	}

	void MonsterAttackObject::Update()
	{
		Vector3 vPosition = m_pOwner->GetComponent<Transform>()->GetPosition();
		GetComponent<Transform>()->SetPosition(vPosition);

		GameObject::Update();
	}

	void MonsterAttackObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void MonsterAttackObject::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		update_state();
	}

	void MonsterAttackObject::update_state()
	{
		Animator* pAnimator = GetComponent<Animator>();
		if (!pAnimator || pAnimator->TrySendPacket())
			return;

		Protocol::S_STATE pkt;

		UCHAR cLayer = (UCHAR)eLayerType::MonsterAttack;
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cDir = m_iDir > 0 ? 1 : 0;
		UCHAR cAnimIdx = pAnim->GetCurIndex();

		pkt.set_state_value((cDir << 8) | cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneName()));
	}

	void MonsterAttackObject::SetMonsterAttack(const tMonsterAttack& _tAttackInfo)
	{
		//데미지
		GetScript<MonsterAttackScript>()->SetAttackInfo(_tAttackInfo.tAttackInfo);
		Transform* pTransform = GetComponent<Transform>();
		
		pTransform->SetPosition(_tAttackInfo.vPosition);
		pTransform->SetRotation(_tAttackInfo.vRoatate);

		//오프셋
		int iDir = m_pOwner->GetDir();
		Collider2D* pCollider = GetComponent< Collider2D>();
		pCollider->SetCenter(Vector2(_tAttackInfo.vOffset.x * iDir, _tAttackInfo.vOffset.y));
		pCollider->SetSize(_tAttackInfo.vScale);

		GetComponent<Collider2D>()->SetActive(true);
	}

}