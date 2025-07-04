#include "WStone.h"
#include "WResources.h"
#include "WStoneScript.h"
#include "WAnimator.h"
#include "WSceneManger.h"
namespace W
{
	UINT Stone::CREATE_ID = 0;

	Stone::Stone()
	{
		
		AddComponent<StoneScript>();
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.8f, 0.8f));

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"stone_left", Vector2(0.0f, 0.0f), Vector2(69.0f, 106.0f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

		pAnim->Play(L"stone_left", true);

	}
	Stone::~Stone()
	{

	}

	void Stone::Initialize()
	{

	}

	void Stone::Update()
	{
		GameObject::Update();
	}
	void Stone::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void Stone::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		update_state();
	}

	void Stone::update_state()
	{
		Animator* pAnimator = GetComponent<Animator>();

		if (!pAnimator->TrySendPacket())
			return;

		Protocol::S_STATE pkt;

		UCHAR cLayer = (UCHAR)GetLayerType();
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cDir = 1;
		UCHAR cAnimIdx = pAnim->GetCurIndex();
		bool bRender = true;

		pkt.set_state_value(bRender << 16 | (cDir << 8) | cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneID()));
	}
	
}