#include "WUltimateShuriken.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WCollider2D.h"
#include "WTime.h"
#include "WAttackScript.h"
#include "WSceneManger.h"
#include "WPlayer.h"

namespace W
{
	UINT UltimateShuriken::CREATE_ID = 0;

	UltimateShuriken::UltimateShuriken()
	{


		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"shuriken", Vector2(0.f, 0.f), Vector2(581.f, 252.f), 6, Vector2(600.f, 600.f), Vector2(0.0f, 0.f), 0.05f);
		pAnim->Play(L"shuriken", true);

		AddComponent<Collider2D>();

		GetComponent<Transform>()->SetScale(5.f, 5.f, 0.f);
		Collider2D* pCOll = GetComponent<Collider2D>();
		pCOll->SetSize(Vector2(0.7f, 0.4f));

		AttackScript* pScript = AddComponent<AttackScript>();
		//pScript->SetDir(m_iDir);
		//최대 15명의 적 공격가능
		pScript->SetDeleteTime(7.f);
		pScript->SetAbleAttackCount(15);
		pScript->SetDeleteObject(false);

	}

	UltimateShuriken::~UltimateShuriken()
	{

	}
	void UltimateShuriken::Initialize()
	{
		GetComponent<Transform>()->SetScale(5.f, 5.f, 0.f);
		Collider2D* pCOll = GetComponent<Collider2D>();
		pCOll->SetSize(Vector2(0.7f, 0.4f));

		AttackScript* pScript = GetComponent<AttackScript>();
	
		pScript->SetStayObject(3, 0.3f);
	}
	void UltimateShuriken::Update()
	{
		Vector3 vPos = GetComponent<Transform>()->GetPosition();
		vPos.x += (Time::DeltaTime() * m_vDir.x * 9.f);
		vPos.y += (Time::DeltaTime() * m_vDir.y * 9.f);

		GetComponent<Transform>()->SetPosition(vPos);

		float fRadian = atan2f(m_vDir.y, m_vDir.x);
		if (m_vDir.x > 0.f)
			fRadian -= XM_2PI;
		else
			fRadian += XM_2PI;

		GetComponent<Transform>()->SetRotation(0.f, 0.f, fRadian);

		GameObject::Update();

	}
	void UltimateShuriken::LateUpdate()
	{
		GameObject::LateUpdate();

	}

	void UltimateShuriken::SetStartPos(Vector2 _vPos)
	{
		Transform* pTransform = GetComponent<Transform>();
		pTransform->SetPosition(0.f, 0.f, 0.f);
		
		Vector3 vTemPos = GetPlayer()->GetComponent<Transform>()->GetPosition();
		Vector2 vCamPos = Vector2(vTemPos.x, vTemPos.y);
		
		vCamPos += (_vPos * 5.f);
		
		Vector3 vPosition = pTransform->GetPosition();
		pTransform->SetPosition(vCamPos.x, vCamPos.y,vPosition.z);
		
		Vector3 vPos = pTransform->GetPosition();
		vPos.z = 0.f;
		Vector3 vTargetPos = m_pTarget->GetComponent<Transform>()->GetPosition();
		vTargetPos.z = 0.f;
		
		Vector3 vDiff = vTargetPos - vPos;
		vDiff.Normalize();
		m_vDir = Vector2(vDiff.x,vDiff.y);
	}

	void UltimateShuriken::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		update_state();
	}

	void UltimateShuriken::update_state()
	{
		Animator* pAnimator = GetComponent<Animator>();
		if (!pAnimator->TrySendPacket())
			return;

		Protocol::S_STATE pkt;

		UCHAR cLayer = (UCHAR)eLayerType::AttackObject;
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cDir = 1;
		UCHAR cAnimIdx = pAnim->GetCurIndex();

		pkt.set_state_value((cDir << 8) | cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneName()));
	}


}