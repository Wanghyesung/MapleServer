#include "WEventManager.h"
#include "WSceneManger.h"
#include "WSkillManager.h"
#include "WObjectPoolManager.h"
#include "..\Engine\WPlayerAttackObject.h"
#include "..\Engine\WMonsterAttackObject.h"
#include "WPlayerScript.h"
#include "WSkillState.h"
#include "..\IOCP_SERVER\Room.h"
#include "WInput.h"
#include "GameObject.pb.h"
#include "ClientPacketHandler.h"
#include "WAnimator.h"

namespace W
{
	std::function<void(DWORD_PTR, DWORD_PTR, LONG_PTR)> EventManager::m_arrFunction[(UINT)EVENT_TYPE::END] = {};

	std::vector<tEvent> EventManager::m_vecEvent[2] = {};
	std::vector<GameObject*> EventManager::m_vecPlayer_Pool = {};
	std::vector<GameObject*> EventManager::m_vecMonster_Pool = {};
	std::vector<USHORT> EventManager::m_vecInput[5][2] = {};

	RWLock EventManager::m_lock = {};
	RWLock EventManager::m_inputLock = {};

	atomic<int> EventManager::m_iActiveIdx = 1;
	atomic<int> EventManager::m_iActiveInputIdx = 1;

#define ObjectPoolPosition 2000.f

	void EventManager::Initialize()
	{
		m_arrFunction[(UINT)EVENT_TYPE::CREATE_PLAYER] = create_player;
		//m_arrFunction[(UINT)EVENT_TYPE::DELETE_PLAYER] = delete_;
		m_arrFunction[(UINT)EVENT_TYPE::UPDATE_INPUT] = update_input;
		m_arrFunction[(UINT)EVENT_TYPE::CREATE_OBJECT] = create_object;
		m_arrFunction[(UINT)EVENT_TYPE::DELET_OBJECT] = delete_object;
		m_arrFunction[(UINT)EVENT_TYPE::ERASE_OBJECT] = erase_object;
		m_arrFunction[(UINT)EVENT_TYPE::CHANGE_STATE] = chanage_state;
		m_arrFunction[(UINT)EVENT_TYPE::CHANGE_PLAYER_STATE] = change_player_fsmstate;
		m_arrFunction[(UINT)EVENT_TYPE::CHANGE_PLAYER_SKILL] = change_player_skillstate;
		m_arrFunction[(UINT)EVENT_TYPE::ADD_PLAYER_SKILL] = add_player_skillstate;
		m_arrFunction[(UINT)EVENT_TYPE::INIT_PLAYER_SKILL] = init_player_skill;
		m_arrFunction[(UINT)EVENT_TYPE::RELEASE_PLAYER_SKILL] = release_player_skill;
		m_arrFunction[(UINT)EVENT_TYPE::CHANGE_MONSTER_STATE] = change_monster_fsmstate;
		m_arrFunction[(UINT)EVENT_TYPE::ADD_PLAYER_POOL] = add_player_pool;
		m_arrFunction[(UINT)EVENT_TYPE::ADD_MONSTER_POOL] = add_monster_pool;
		m_arrFunction[(UINT)EVENT_TYPE::CHANGE_SCENE] = change_scene;
		m_arrFunction[(UINT)EVENT_TYPE::START_SCENE] = start_scene;
		m_arrFunction[(UINT)EVENT_TYPE::HITCH_ABNORMAL] = hitch_abnormal;
		m_arrFunction[(UINT)EVENT_TYPE::UP_STAT] = up_stat;
		m_arrFunction[(UINT)EVENT_TYPE::RESTORE] = restore;
	}

	void EventManager::Update()
	{
		pool_excute(); //오브젝트 풀

		//더블버퍼링
		{
			WLock lock_guard(m_lock);
			m_iActiveIdx = 1 - m_iActiveIdx;
		}

		std::vector<tEvent>& vecActiveEvent = m_vecEvent[m_iActiveIdx];
		for (int i = 0; i < vecActiveEvent.size(); ++i)
		{
			excute(vecActiveEvent[i]);
		}

		vecActiveEvent.clear();
	}

	void EventManager::AddEvent(const tEvent& _tEve)
	{
		WLock lock_guard(m_lock);
		m_vecEvent[1 - m_iActiveIdx].push_back(_tEve);
	}
	

	void EventManager::pool_excute()
	{
		for (int i = 0; i < m_vecPlayer_Pool.size(); ++i)
		{
			ObjectPoolManager::AddObjectPool(m_vecPlayer_Pool[i]->GetName(), m_vecPlayer_Pool[i]);
			SceneManger::Erase(m_vecPlayer_Pool[i]);

			dynamic_cast<PlayerAttackObject*>(m_vecPlayer_Pool[i])->Off();
		}
		
		for (int i = 0; i < m_vecMonster_Pool.size(); ++i)
		{
			ObjectPoolManager::AddObjectPool(m_vecMonster_Pool[i]->GetName(), m_vecMonster_Pool[i]);
			SceneManger::Erase(m_vecMonster_Pool[i]);
		}

		m_vecPlayer_Pool.clear();
		m_vecMonster_Pool.clear();
	}



	void EventManager::excute(const tEvent& _tEve)
	{
		m_arrFunction[(UINT)_tEve.eEventType](_tEve.lParm,_tEve.wParm, _tEve.accParm);
	}

	void EventManager::create_object(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		GameObject* pObj = (GameObject*)_lParm;
		eLayerType eLayer = (eLayerType)_wParm;

		SceneManger::AddGameObject(pObj->GetSceneName(), eLayer, pObj);

		UINT iCreateID = pObj->GetCreateID();
		UINT iObjectID = pObj->GetObjectID();
	
		Protocol::S_CREATE pkt;
		Protocol::ObjectInfo* tInfo = pkt.mutable_object_info();
		
		if (pObj->IsPoolObject())
			tInfo->set_object_name(WstringToString(pObj->GetName()));
		
		
		tInfo->set_layer_createid_id((UCHAR)eLayer << 24 | iCreateID << 16 | iObjectID);
		
		Vector3 vPosition = pObj->GetComponent<Transform>()->GetPosition();
		tInfo->set_x(vPosition.x);	tInfo->set_y(vPosition.y);	tInfo->set_z(vPosition.z);
		
		UCHAR cDir = 1 ;
		UCHAR cAnimIdx = 0;
		UCHAR bRender = pObj->IsRender();
		Animator* pAnim = pObj->GetComponent<Animator>();
		if (pAnim)
			cAnimIdx = 0;
		
		tInfo->set_state_value((bRender<<16) | (cDir << 8) | cAnimIdx);
		
		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(pObj->GetSceneName()));
	}

	void EventManager::delete_object(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		GameObject* pObj = (GameObject*)_lParm;

		Scene* pScene = SceneManger::FindScene(pObj->GetSceneName());
		pScene->EraseObject(pObj->GetLayerType(), pObj);

		UINT iObjectID = pObj->GetObjectID();
		Protocol::S_DELETE pkt;
	
		pkt.set_layer_deleteid((UINT)pObj->GetLayerType() << 24 | iObjectID & 0x00FFFFFF);
		pkt.set_pool_object(pObj->IsPoolObject());

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(pObj->GetSceneName()));

		delete pObj;
	}

	void EventManager::erase_object(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		GameObject* pObj = (GameObject*)_lParm;

		Scene* pScene = SceneManger::FindScene(pObj->GetSceneName());
		pScene->EraseObject(pObj->GetLayerType(), pObj);

		UINT iObjectID = pObj->GetObjectID();
		Protocol::S_DELETE pkt;
		pkt.set_layer_deleteid((UCHAR)pObj->GetLayerType() << 24 | iObjectID);
		pkt.set_pool_object(pObj->IsPoolObject());

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(pObj->GetSceneName()));
	}

	void EventManager::update_input(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		UINT iPlayerID = (UINT)_lParm;
		{
			WLock lock_guard(m_inputLock);
			Input::Update_Key(iPlayerID, m_vecInput[iPlayerID][m_iActiveInputIdx]);

			m_vecInput[iPlayerID][m_iActiveInputIdx].clear();
			m_iActiveInputIdx.exchange(1 - m_iActiveInputIdx);
		}
	}

	void EventManager::create_player(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		UINT iPlayerID = (UINT)_lParm;

		Player* pPlayer = new Player();
		pPlayer->m_iPlayerID = iPlayerID;
		pPlayer->SetObjectID(iPlayerID);
		pPlayer->Initialize();
		SceneManger::AddPlayerScene(pPlayer, L"Valley");
	}

	void EventManager::add_player_pool(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		GameObject* pObj = (GameObject*)_lParm;

		m_vecPlayer_Pool.push_back(pObj);
	}

	void EventManager::add_monster_pool(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		GameObject* pObj = (GameObject*)_lParm;

		m_vecMonster_Pool.push_back(pObj);
	}

	void EventManager::change_scene(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		UINT iPlayerID = (UINT)(_lParm);
		const wstring& strNextScene = *reinterpret_cast<wstring*>(_accParm);
		GameObject* pPlayer = SceneManger::FindPlayer(iPlayerID);

		wstring strPrevScene = pPlayer->GetSceneName(); //swap 후 이름이 변경되지 않게 복사
		
		SceneManger::SwapPlayer(pPlayer, strPrevScene, strNextScene);

		SceneManger::SendEnterScene(iPlayerID, strNextScene);


		//이전 맵에 플레이어 삭제됐다고 알리기
		Protocol::S_DELETE pkt;
		pkt.set_layer_deleteid((UCHAR)eLayerType::Player << 24 | iPlayerID);
		pkt.set_pool_object(pPlayer->IsPoolObject());

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(strPrevScene));

		delete &strNextScene;
	}

	void EventManager::start_scene(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		UINT iPlayerID = (UINT)(_lParm);
		const wstring& strScene = *reinterpret_cast<wstring*>(_accParm);

		SceneManger::SendEnterScene(iPlayerID, strScene);

		delete& strScene;
	}


	void EventManager::chanage_state(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		GameObject* pObj = (GameObject*)_lParm;
		GameObject::eState eState = (GameObject::eState)_wParm;

		pObj->SetState(eState);
	}

	void EventManager::change_player_fsmstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		PlayerFSM* pFSM = (PlayerFSM*)_lParm;
		Player::ePlayerState eState = (Player::ePlayerState)_wParm;
		pFSM->ChangeState(eState);
	}

	void EventManager::change_player_skillstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		UINT iPlayerID = (UINT)_lParm;
		Player::ePlayerSkill _ePlayerSkill = (Player::ePlayerSkill)_wParm;

		SkillManager::SetActiveSkill(iPlayerID, _ePlayerSkill);
	}

	void EventManager::add_player_skillstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		SkillState* pSkillState = (SkillState*)_lParm;
		UINT iPlayerID = pSkillState->GetPlayer()->GetPlayerID();
		SkillManager::AddSkill(iPlayerID, pSkillState);
	}

	void EventManager::init_player_skill(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		UINT iPlayerID = (UINT)_lParm;
		PlayerSkill* pPlayerSkill = (PlayerSkill*)_wParm;

		SkillManager::InitSkill(iPlayerID, pPlayerSkill);
	}

	void EventManager::release_player_skill(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		UINT iPlayerID = (UINT)_lParm;

		SkillManager::ReleaseSkill(iPlayerID);
	}

	void EventManager::change_monster_fsmstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		MonsterFSM* pFSM = (MonsterFSM*)_lParm;
		Monster::eMonsterState eState = (Monster::eMonsterState)_wParm;
		pFSM->ChangeState(eState);
	}

	void EventManager::hitch_abnormal(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		GameObject* pObj = (GameObject*)_lParm;
		BattleManager::eAbnormalType eType = (BattleManager::eAbnormalType)_wParm;
		float fAccValue = _accParm;

		BattleManager::excute_abnormal(eType, pObj, fAccValue);
	}

	void EventManager::restore(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		GameObject* pObj = (GameObject*)_lParm;
		BattleManager::eAbnormalType eType = (BattleManager::eAbnormalType)_wParm;
		float fAccStat = (float)_accParm;

		BattleManager::restore(pObj, eType, fAccStat);
	}

	void EventManager::up_stat(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm)
	{
		GameObject* pObj = (GameObject*)_lParm;
		BattleManager::eUpStatType eType = (BattleManager::eUpStatType)_wParm;
		float fAccStat = (float)_accParm;
		
		BattleManager::excute_stat(eType, pObj, fAccStat);
	}


	void EventManager::DeleteObject(GameObject* _pObj)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pObj;

		eve.eEventType = EVENT_TYPE::DELET_OBJECT;
		AddEvent(eve);
	}

	void EventManager::EraseObject(GameObject* _pObj)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pObj;

		eve.eEventType = EVENT_TYPE::ERASE_OBJECT;
		AddEvent(eve);
	}

	void EventManager::Update_Input(UINT _iPlayerID, const std::vector<USHORT>& _vecInput)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iPlayerID;
		
		{
			WLock lock_guard(m_inputLock);

			for(int i = 0; i<_vecInput.size(); ++i)
				m_vecInput[_iPlayerID][m_iActiveInputIdx].push_back(_vecInput[i]);
		}

		eve.eEventType = EVENT_TYPE::UPDATE_INPUT;
		AddEvent(eve);
	}

	void EventManager::CreatePlayer(UINT _ID)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_ID;

		eve.eEventType = EVENT_TYPE::CREATE_PLAYER;
		AddEvent(eve);
	}

	void EventManager::AddPlayerPool(GameObject* _pObj)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pObj;
	
		eve.eEventType = EVENT_TYPE::ADD_PLAYER_POOL;
	
		Vector3 vPosition = _pObj->GetComponent<Transform>()->GetPosition();
		vPosition.x += ObjectPoolPosition;
		vPosition.y += ObjectPoolPosition;
		_pObj->GetComponent<Transform>()->SetPosition(vPosition);

		AddEvent(eve);
	}

	void EventManager::AddMonsterPool(GameObject* _pObj)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pObj;
		
		eve.eEventType = EVENT_TYPE::ADD_MONSTER_POOL;

		Vector3 vPosition = _pObj->GetComponent<Transform>()->GetPosition();
		vPosition.x += ObjectPoolPosition;
		vPosition.y += ObjectPoolPosition;
		_pObj->GetComponent<Transform>()->SetPosition(vPosition);

		AddEvent(eve);
	}

	void EventManager::StartScene(UINT _iPlayerID, const wstring& _strSceneName)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iPlayerID;
		eve.accParm = (LONG_PTR)new wstring(_strSceneName);

		eve.eEventType = EVENT_TYPE::START_SCENE;
		AddEvent(eve);
	}

	void EventManager::ChanageScene(UINT _iPlayerID, const wstring& _strNextSceneName)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iPlayerID;
		eve.accParm = (LONG_PTR)new wstring(_strNextSceneName);

		eve.eEventType = EVENT_TYPE::CHANGE_SCENE;
		AddEvent(eve);
	}

	void EventManager::ChanageState(GameObject* _pObj, GameObject::eState _eState)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pObj;
		eve.wParm = (DWORD_PTR)_eState;;

		eve.eEventType = EVENT_TYPE::CHANGE_STATE;
		
		AddEvent(eve);
	}

	void EventManager::ChangePlayerFSMState(PlayerFSM* _pFSM, Player::ePlayerState _ePlayerState)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pFSM;//보낼 주소
		eve.wParm = (DWORD_PTR)_ePlayerState;//다음 상태값
		eve.eEventType = EVENT_TYPE::CHANGE_PLAYER_STATE;

		AddEvent(eve);
	}
	void EventManager::ChangePlayerSkillState(UINT _iPlayerID, Player::ePlayerSkill _ePlayerSkill)
	{
		tEvent eve = {};
		
		eve.lParm = (DWORD_PTR)_iPlayerID;
		eve.wParm = (DWORD_PTR)_ePlayerSkill;
		//eve.wParm = (DWORD_PTR)_ePlayerSkill;
		eve.eEventType = EVENT_TYPE::CHANGE_PLAYER_SKILL;
		AddEvent(eve);
	}
	void EventManager::AddPlayerSkillState(SkillState* _pSkillState)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pSkillState;
		eve.eEventType = EVENT_TYPE::ADD_PLAYER_SKILL;
		AddEvent(eve);
	}
	void EventManager::InitPlayerSkill(UINT _iPlayerID, PlayerSkill* _pPlayerSkill)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iPlayerID;
		eve.wParm = (DWORD_PTR)_pPlayerSkill;

		eve.eEventType = EVENT_TYPE::INIT_PLAYER_SKILL;
		AddEvent(eve);
	}
	void EventManager::ReleasePlayerSkill(UINT _iPlayerID)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iPlayerID;

		eve.eEventType = EVENT_TYPE::RELEASE_PLAYER_SKILL;
		AddEvent(eve);
	}

	void EventManager::ChangeMonsterFSMState(MonsterFSM* _pFSM, Monster::eMonsterState _eMonsterState)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pFSM;//보낼 주소
		eve.wParm = (DWORD_PTR)_eMonsterState;//다음 상태값
		eve.eEventType = EVENT_TYPE::CHANGE_MONSTER_STATE;

		AddEvent(eve);
	}

	void EventManager::CreateObject(GameObject* _pObj, eLayerType _eLayer)
	{
		tEvent eve = {};
		eve.eEventType = EVENT_TYPE::CREATE_OBJECT;
		eve.lParm = (DWORD_PTR)_pObj;
		eve.wParm = (DWORD_PTR)_eLayer;

		AddEvent(eve);
	}

	void EventManager::HitchAbnormal(GameObject* _pObj, BattleManager::eAbnormalType _eType, float _fAccStat)
	{
		tEvent eve = {};
		eve.eEventType = EVENT_TYPE::HITCH_ABNORMAL;
		eve.lParm = (DWORD_PTR)_pObj;
		eve.wParm = (DWORD_PTR)_eType;
		eve.accParm = _fAccStat;

		AddEvent(eve);
	}

	void EventManager::Restore(GameObject* _pObj, BattleManager::eAbnormalType _eType, float _fAccStat)
	{
		tEvent eve = {};
		eve.eEventType = EVENT_TYPE::RESTORE;
		eve.lParm = (DWORD_PTR)_pObj;
		eve.wParm = (DWORD_PTR)_eType;
		eve.accParm = (LONG_PTR)_fAccStat;

		AddEvent(eve);
	}

	void EventManager::UpStat(GameObject* _pObj, BattleManager::eUpStatType _eType, float _fAccStat)
	{
		tEvent eve = {};
		eve.eEventType = EVENT_TYPE::UP_STAT;
		eve.lParm = (DWORD_PTR)_pObj;
		eve.wParm = (DWORD_PTR)_eType;
		eve.accParm = (LONG_PTR)_fAccStat;

		AddEvent(eve);
	}

}