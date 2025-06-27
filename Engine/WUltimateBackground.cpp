#include "WUltimateBackground.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WAttackScript.h"
#include "WSceneManger.h"
#include "WPlayer.h"
namespace W
{
	UINT UltimateBackground::CREATE_ID = 0;

	UltimateBackground::UltimateBackground()
	{
	
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"ultimate1", Vector2(0.f, 0.f), Vector2(1412.f, 812.f), 10, Vector2(1500.f, 1500.f), Vector2(0.0f, 0.f), 0.065f);
		pAnim->FindAnimation(L"ultimate1")->Create(L"ultimate1", Vector2(0.f, 812.f), Vector2(1412.f, 812.f), 6, Vector2(1500.f, 1500.f), Vector2(0.0f, 0.f), 0.065f);

		//mr->SetMaterial(pMater);

		AddComponent<Collider2D>()->SetSize(Vector2(0.f, 0.f));
		AddComponent<AttackScript>()->SetDeleteTime(20.f);
		GetComponent<AttackScript>()->SetDeleteObject(false);
	}
	UltimateBackground::~UltimateBackground()
	{
	}
	void UltimateBackground::Initialize()
	{
		GetComponent<Transform>()->SetScale(Vector3(15.5f, 15.f, 0.f));

		GetComponent<Animator>()->Play(L"ultimate1", true);
	}
	void UltimateBackground::Update()
	{
		
		GameObject::Update();
	}
	void UltimateBackground::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void UltimateBackground::UpdatePacket()
	{
		update_state();
	}

	void UltimateBackground::update_state()
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