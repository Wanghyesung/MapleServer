#include "WMonsterHP.h"
#include "WMonster.h"
#include "WMonsterScript.h"
#include "WMonsterBackHP.h"
#include "WMonsterManager.h"
#include "WEventManager.h"
#include "WSceneManger.h"
namespace W
{
	UINT MonsterHP::HP_COUNT = 0;
	UINT MonsterHP::CREATE_ID = 0;

	MonsterHP::MonsterHP(bool bBoss):
		m_pOwner(nullptr),
		m_fHP(100.f),
		m_iHPCount(HP_COUNT),
		m_bActive(true),
		m_bBoss(bBoss)
	{
		++HP_COUNT;
		std::wstring strNum =  std::to_wstring(m_iHPCount);

		if (!m_bBoss)
		{
			GetComponent<Transform>()->SetScale(1.f * 0.66f, 1.f * 0.13f, 0.f);
		}
		else
		{
			GetComponent<Transform>()->SetScale(1.2f * 7.6f, 1.2f * 0.4f, 0.f);
			GetComponent<Transform>()->SetPosition(0.f, 3.5f, -2.f);
		}

	}
	MonsterHP::~MonsterHP()
	{
		
	}
	void MonsterHP::Initialize()
	{
		m_pHPBack = new MonsterBackHP(m_bBoss);
		m_pHPBack->SetSceneName(GetSceneName());
		m_pHPBack->SetHP(this);
		if (!m_bBoss)
			EventManager::CreateObject(m_pHPBack, eLayerType::Object);
		else
			EventManager::CreateObject(m_pHPBack, eLayerType::UI);
	}
	void MonsterHP::Update()
	{
		if (!m_bActive)
			return;
		
		if (m_fHP <= 0.f)
		{
			m_bActive = false;
			EventManager::DeleteObject(this);
			EventManager::DeleteObject(m_pHPBack);
			return;
		}

		if (!m_bBoss)
		{
			Vector3 vPosition = m_pOwner->GetComponent<Transform>()->GetPosition();
			vPosition.z -= 0.02f;
			vPosition.y += 0.5f;
			GetComponent<Transform>()->SetPosition(vPosition);
		}
		MonsterScript* pScript = m_pOwner->GetScript<MonsterScript>();
		tObjectInfo tObjInfo = pScript->GetObjectInfo();
		m_fHP = tObjInfo.fHP;

		GameObject::Update();
	}
	void MonsterHP::LateUpdate()
	{
		if (!m_bActive)
			return;

		GameObject::LateUpdate();
	}

	void MonsterHP::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();
		update_hp();
	}
	
	void MonsterHP::DeleteHP()
	{
		MonsterManager::AddDeleteObject(m_pHPBack);
		MonsterManager::AddDeleteObject(this);
		
	}
	void MonsterHP::update_hp()
	{
		Protocol::S_STATE pkt;

		UCHAR cLayer = (UCHAR)GetLayerType();
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		//이거 패킷 이름 set_int , set_string으로 변경하기
		pkt.set_anim(m_fHP);
		
		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneName()));
	}
}