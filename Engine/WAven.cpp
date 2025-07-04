#include "WAven.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WAttackScript.h"
#include "WTime.h"
#include "WEventManager.h"
#include "WSceneManger.h"
namespace W
{
	UINT Aven::CREATE_ID = 0;

	Aven::Aven()
	{
		GetComponent<Transform>()->SetScale(2.f * 0.72, 2.f * 0.72, 0.f);

	}

	Aven::~Aven()
	{

	}

	void Aven::Initialize()
	{
		AttackScript* pScript = AddComponent<AttackScript>();
		pScript->SetStayObject(1, 10.f);

		Collider2D* pCollider = AddComponent<Collider2D>();
		GetComponent<Transform>()->SetScale(1.f, 1.f, 0.f);
		pCollider->SetSize(Vector2(0.5f, 0.5f));

	}

	void Aven::Update()
	{
		GameObject::Update();

		Vector3 vRotation = GetComponent<Transform>()->GetRotation();
		vRotation.z += XM_PI  * Time::DeltaTime() * 6;
		GetComponent<Transform>()->SetRotation(vRotation);
	}

	void Aven::LateUpdate()
	{
		Transform* pTr = GetComponent<Transform>();
		Vector3 vPosition = pTr->GetPosition();
		vPosition.x += (m_iDir * 8.f * Time::DeltaTime());
		pTr->SetPosition(vPosition);

		GameObject::LateUpdate();
	}

	void Aven::Off()
	{
		PlayerAttackObject::Off();
	}

	void Aven::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		update_state();
	}

	void Aven::update_state()
	{
		Animator* pAnimator = GetComponent<Animator>();
		if (!pAnimator || pAnimator->TrySendPacket())
			return;

		Protocol::S_STATE pkt;

		UCHAR cLayer = (UCHAR)eLayerType::AttackObject;
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cDir = m_iDir > 0 ? 1 : 0;
		UCHAR cAnimIdx = pAnim->GetCurIndex();

		pkt.set_state_value((cDir << 8) | cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneID()));
	}
}