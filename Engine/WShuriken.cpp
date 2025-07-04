#include "WShuriken.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WAttackScript.h"
#include "WSceneManger.h"
#include "WTime.h"
#include "WEventManager.h"
#include "GameObject.pb.h"



namespace W
{
	UINT Shuriken::CREATE_ID = 0;
	UINT Shuriken::COUNT = 0;

	Shuriken::Shuriken()
	{
		m_iCount = COUNT;
		++COUNT;

	
		Animator* pAnimator = AddComponent<Animator>();
		
		pAnimator->Create(L"shuriken_left", Vector2(0.0f, 0.0f), Vector2(48.0f, 9.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.05f);
		pAnimator->Create(L"shuriken_right", Vector2(48.0f, 0.0f), Vector2(-48.0f, 9.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.05f);
	}

	Shuriken::~Shuriken()
	{

	}

	void Shuriken::Initialize()
	{
		AddComponent<AttackScript>();

		Collider2D* pCollider = AddComponent<Collider2D>();
		GetComponent<Transform>()->SetScale(1.f, 1.f, 0.f);
		pCollider->SetSize(Vector2(0.5f, 0.5f));
	}

	//이동, 삭제시간, attackscript말고 각각의 클래스에서
	void Shuriken::Update()
	{	
		GameObject::Update();
	}

	void Shuriken::LateUpdate()
	{
		Transform* pTr =GetComponent<Transform>();
		Vector3 vPosition = pTr->GetPosition();
		vPosition.x += (m_iDir * 16.f * Time::DeltaTime());
		pTr->SetPosition(vPosition);

		GameObject::LateUpdate();

	}

	void Shuriken::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		update_state();
	}

	void Shuriken::SetDir(int _iDir)
	{
		m_iDir = _iDir;
		AttackScript* pScript = GetScript<AttackScript>();
		pScript->SetDir(m_iDir);

		Animator* pAnimator = GetComponent<Animator>();
		std::wstring strDir;
		if (m_iDir > 0)
			strDir = L"_right";
		else
			strDir = L"_left";
		pAnimator->Play(L"shuriken" + strDir, true);
	}
	void Shuriken::Off()
	{
		PlayerAttackObject::Off();
	}

	void Shuriken::update_state()
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