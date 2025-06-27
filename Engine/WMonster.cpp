#include "WMonster.h"
#include "WSceneManger.h"
#include "WCollisionManager.h"
#include "WObjectPoolManager.h"
#include "WAnimator.h"
namespace W
{
	UINT Monster::CREATE_ID = 0;

	Monster::Monster():
		m_iDir(1),
		m_eMonsterState(eMonsterState::stand),
		m_strCurStateName(L"_stand"),
		m_vecItemNames{},
		m_iCreateCount(1)
		//m_mapAttackObjs{}
	{
		
	}
	Monster::~Monster()
	{
		
	}
	void Monster::Initialize()
	{
		
	}
	void Monster::Update()
	{
		if (m_bDead)
			return;

		GameObject::Update();
	}
	void Monster::LateUpdate()
	{
		if (m_bDead)
			return;

		GameObject::LateUpdate();
	}

	void Monster::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();

		update_state();
	}

	void Monster::SetItem(const std::vector<std::wstring>& _vecItemNames)
	{
		for (int i = 0; i < _vecItemNames.size(); ++i)
			m_vecItemNames.push_back(_vecItemNames[i]);
	}

	void Monster::AddMonsterSkill(MonsterAttackObject* _pObj)
	{
		ObjectPoolManager::AddObjectPool(_pObj->GetName(), _pObj);
	}

	MonsterAttackObject* Monster::GetMonsterSkill(const std::wstring& _strName)
	{
		GameObject* pGameObj = ObjectPoolManager::PopObject(_strName);
		pGameObj->SetSceneName(GetSceneName());
		return static_cast<MonsterAttackObject*>(pGameObj);
	}

	void Monster::update_state()
	{
		Animator* pAnimator = GetComponent<Animator>();

		bool bRender = IsDead();

		if (!pAnimator->TrySendPacket() && m_bPrevDead == bRender)
			return;

		m_bPrevDead = m_bDead;

		Protocol::S_STATE pkt;
		UCHAR cLayer = (UCHAR)eLayerType::Monster;
		UINT iObjectID = GetObjectID();
		pkt.set_layer_id((cLayer << 24) | iObjectID);

		Animation* pAnim = pAnimator->GetActiveAnimation();
		UCHAR cDir = GetDir() > 0 ? 1 : 0;
		UCHAR cAnimIdx = pAnim->GetCurIndex();

		pkt.set_state_value(!bRender << 16 | (cDir << 8) | cAnimIdx);
		pkt.set_state(WstringToString(pAnim->GetKey()));

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(GetSceneName()));
	}

	

}