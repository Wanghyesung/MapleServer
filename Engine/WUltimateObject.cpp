#include "WUltimateObject.h"
#include "WAttackScript.h"
#include "WSceneManger.h"
#include "WTime.h"
#include "WAnimator.h"

#include "WEventManager.h"
namespace W
{
	UINT UltimateObject::CREATE_ID = 0;

	UltimateObject::UltimateObject():
		m_fCurResetTime(0.f),
		m_fResetTime(0.25f),
		m_iCurFrame(0),
		m_iAttackFrame(24),
		m_iEndFrame(49),
		m_iCreateCount(0)
	{
		

		Animator* pAnim = AddComponent<Animator>();
		
		pAnim->Create(L"ultimate", Vector2(0.f, 0.f), Vector2(1456.f, 860.f), 10, Vector2(1500.f, 1500.f), Vector2(0.0f, 0.f), 0.04f);
		pAnim->FindAnimation(L"ultimate")->Create(L"ultimate", Vector2(0.f, 860.f), Vector2(1456.f, 860.f), 10, Vector2(1500.f, 1500.f), Vector2(0.0f, 0.f), 0.04f);
		pAnim->FindAnimation(L"ultimate")->Create(L"ultimate", Vector2(0.f, 1720.f), Vector2(1456.f, 860.f), 10, Vector2(1500.f, 1500.f), Vector2(0.0f, 0.f),0.04f);
		pAnim->FindAnimation(L"ultimate")->Create(L"ultimate", Vector2(0.f, 2580.f), Vector2(1456.f, 860.f), 10, Vector2(1500.f, 1500.f), Vector2(0.0f, 0.f),0.04f);
		pAnim->FindAnimation(L"ultimate")->Create(L"ultimate", Vector2(0.f, 3440.f), Vector2(1456.f, 860.f), 10, Vector2(1500.f, 1500.f), Vector2(0.0f, 0.f),0.04f);

		GetComponent<Transform>()->SetScale(15.5f, 15.5f, 0.f);


		//2초 동안 총 8번의 공격 공격당 7대(0.25초당 초기화)
		AddComponent<AttackScript>();
		GetScript<AttackScript>()->SetStayObject(7, m_fResetTime);
		GetScript<AttackScript>()->SetDeleteTime(m_fResetTime * 8.f * 2.f);
		//최대 15명 공격 가능
		GetScript<AttackScript>()->SetAbleAttackCount(15);

	}
	UltimateObject::~UltimateObject()
	{

	}
	void UltimateObject::Initialize()
	{
		GetComponent<Animator>()->Play(L"ultimate", false);
	}
	void UltimateObject::Update()
	{
		//위치 잡기
		//Vector3 vCamPos = renderer::MainCamera->GetOwner()->GetComponent<Transform>()->GetPosition();
		//GetComponent<Transform>()->SetPosition(Vector3(vCamPos.x, vCamPos.y, -4.f));

		m_iCurFrame = GetComponent<Animator>()->GetActiveAnimation()->GetCurIndex();
		if (m_iCurFrame >= m_iAttackFrame)
		{
			if (m_iCreateCount == 0)
			{
				m_iCreateCount = 1;
				GetComponent<Collider2D>()->SetActive(true);
			}
			attack();
		}

		GameObject::Update();
	}
	void UltimateObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void UltimateObject::Off()
	{
		m_iCreateCount = 0;
		m_iCurFrame = 0;
		m_fCurResetTime = 0.f;
	
		PlayerAttackObject::Off();
	}

	void UltimateObject::attack()
	{
		m_fCurResetTime += Time::DeltaTime();

		if (m_fCurResetTime >= m_fResetTime)
		{
			m_fCurResetTime = 0.f;
			GetScript< AttackScript>()->ResetAttackCount();
		}
	}

	void UltimateObject::UpdatePacket()
	{
		update_state();
	}

	void UltimateObject::update_state()
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
		GRoom.GetPersonByID(GetPlayer()->GetPlayerID())->Send(pSendBuffer);
	}

}