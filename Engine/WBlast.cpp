#include "WBlast.h"
#include "WAttackScript.h"
#include "WAnimator.h"
#include "WEventManager.h"
#include "WSceneManger.h"
namespace W
{
	UINT Blast::CREATE_ID = 0;

	Blast::Blast()
	{
		Animator* pAnim = AddComponent<Animator>();
		
		pAnim->Create(L"left", Vector2(0.0f, 0.0f), Vector2(695.0f, 506.0f), 23, Vector2(700.f, 700.f), Vector2::Zero, 0.03f);
		pAnim->Create(L"right", Vector2(15290.0f, 0.0f), Vector2(-695.0f, 506.0f), 23, Vector2(700.f, 700.f), Vector2::Zero, 0.03f);

		pAnim->CompleteEvent(L"right") = std::bind(&EventManager::AddPlayerPool, this);
		pAnim->CompleteEvent(L"left") = std::bind(&EventManager::AddPlayerPool, this);


		GetComponent<Transform>()->SetScale(8.f, 8.f, 0.f);
	}
	Blast::~Blast()
	{

	}
	void Blast::Initialize()
	{
		AttackScript* pScript = AddComponent<AttackScript>();
		pScript->SetDir(m_iDir);
		pScript->SetStayObject(5, 0.5f);
		pScript->SetDeleteTime(20.f);

		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.4f, 0.4f));
	}
	void Blast::Update()
	{
		GameObject::Update();
	}
	void Blast::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Blast::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		update_state();
	}

	void Blast::update_state()
	{
		Animator* pAnimator = GetComponent<Animator>();
		if (!pAnimator->TrySendPacket())
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
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneID()));
	}

	void Blast::Off()
	{
		PlayerAttackObject::Off();
	}
	void Blast::SetDir(int _iDir)
	{
		Animator* pAnim = GetComponent<Animator>();
		m_iDir = _iDir;
		if (m_iDir > 0)
			pAnim->Play(L"right", true);
		else
			pAnim->Play(L"left", true);

	}
	
}