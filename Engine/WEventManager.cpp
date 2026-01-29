#include "WEventManager.h"
#include "WSceneManger.h"
#include "WSkillManager.h"
#include "WItemManager.h"
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
	std::function<void(DWORD_PTR, DWORD_PTR, LONG_PTR, OBJECT_DATA)> EventManager::m_arrFunction[(UINT)EVENT_TYPE::END] = {};

	std::vector<tEvent> EventManager::m_vecEvent[2] = {};
	std::vector<GameObject*> EventManager::m_vecPlayer_Pool = {};
	std::vector<GameObject*> EventManager::m_vecMonster_Pool = {};
	std::vector<USHORT> EventManager::m_vecInput[MAXCOUNT][2] = {};

	RWLock EventManager::m_lock = {};
	RWLock EventManager::m_inputLock = {};
	atomic<int> EventManager::m_iActiveIdx = 1;
	atomic<int> EventManager::m_iActiveInputIdx = 1;

#define ObjectPoolPosition 2000.f

	void EventManager::Initialize()
	{
		m_arrFunction[(UINT)EVENT_TYPE::CREATE_PLAYER] = create_player;
		m_arrFunction[(UINT)EVENT_TYPE::DELETE_PLAYER] = delete_player;
		m_arrFunction[(UINT)EVENT_TYPE::UPDATE_INPUT] = update_input;
		m_arrFunction[(UINT)EVENT_TYPE::CREATE_OBJECT] = create_object;
		m_arrFunction[(UINT)EVENT_TYPE::DELET_OBJECT] = delete_object;
		m_arrFunction[(UINT)EVENT_TYPE::ERASE_OBJECT] = erase_object;
		m_arrFunction[(UINT)EVENT_TYPE::CHANGE_STATE] = chanage_state;
		m_arrFunction[(UINT)EVENT_TYPE::CHANGE_PLAYER_STATE] = change_player_fsmstate;
		m_arrFunction[(UINT)EVENT_TYPE::CHANGE_PLAYER_SKILL] = change_player_skillstate;
		m_arrFunction[(UINT)EVENT_TYPE::CHANGE_PLAYER_EQUIP] = change_player_equip;
		m_arrFunction[(UINT)EVENT_TYPE::USING_ITEM] = using_item;
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
		//다 사용한 오브젝트 회수
		pool_execute(); 
		{
			WLock event_lock(m_lock);
			WLock input_lock(m_inputLock);

			m_iActiveIdx = 1 - m_iActiveIdx;
			m_iActiveInputIdx = 1 - m_iActiveInputIdx;
		}
		std::vector<tEvent>& vecActiveEvent = m_vecEvent[m_iActiveIdx];
		for (int i = 0; i < vecActiveEvent.size(); ++i)
			execute(vecActiveEvent[i]);

		vecActiveEvent.clear();
	}

	void EventManager::AddEvent(const tEvent& _tEve)
	{
		WLock lock_guard(m_lock);
		m_vecEvent[1 - m_iActiveIdx].emplace_back(_tEve);
	}
	

	void EventManager::pool_execute()
	{
		for (int i = 0; i < m_vecPlayer_Pool.size(); ++i)
		{
			PlayerAttackObject* pAttackObj = static_cast<PlayerAttackObject*>(m_vecPlayer_Pool[i]);
			if (pAttackObj->IsPool())//이미 들어왔다면
				continue;

			ObjectPoolManager::AddObjectPool(pAttackObj->GetName(), pAttackObj);
			SceneManger::Erase(m_vecPlayer_Pool[i], pAttackObj->GetPlayer()->GetObjectID());

			pAttackObj->Off();
		}
		
		for (int i = 0; i < m_vecMonster_Pool.size(); ++i)
		{
			ObjectPoolManager::AddObjectPool(m_vecMonster_Pool[i]->GetName(), m_vecMonster_Pool[i]);
			SceneManger::Erase(m_vecMonster_Pool[i]);
		}

		m_vecPlayer_Pool.clear();
		m_vecMonster_Pool.clear();
	}



	void EventManager::execute(const tEvent& _tEve)
	{
		m_arrFunction[(UINT)_tEve.eEventType](_tEve.lParm,_tEve.wParm, _tEve.accParm, _tEve.tObjectData);
	}

	void EventManager::create_object(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		GameObject* pObj = (GameObject*)_lParm;
		eLayerType eLayer = (eLayerType)_wParm;
		SceneManger::AddGameObject(pObj->GetSceneID(), eLayer, pObj);

		UCHAR iCreateID = pObj->GetCreateID();
		UCHAR iObjectID = pObj->GetObjectID();
	
		Protocol::S_CREATE pkt;
		Protocol::ObjectInfo* tInfo = pkt.mutable_object_info();
		
		if (pObj->IsPoolObject())
			tInfo->set_object_name(WstringToString(pObj->GetName()));
		tInfo->set_scene_layer_createid_id((UCHAR)pObj->GetSceneID() << 24 | (UCHAR)eLayer << 16 | iCreateID << 8 | iObjectID);
		
		Transform* pTr = pObj->GetComponent<Transform>();
		const Vector3& vPosition = pTr->GetPosition();
		const Vector3& vRotation = pTr->GetRotation();
		Protocol::TransformInfo* tTrInfo = tInfo->mutable_transform();

		tTrInfo->set_p_x(vPosition.x);	tTrInfo->set_p_y(vPosition.y);	tTrInfo->set_p_z(vPosition.z);
		tTrInfo->set_r_x(vRotation.x);	tTrInfo->set_r_y(vRotation.y);	tTrInfo->set_r_z(vRotation.z);
		
		UCHAR cDir = 1 ;
		UCHAR cAnimIdx = 0;
		UCHAR bRender = pObj->IsRender();
		tInfo->set_state_value((bRender<<16) | (cDir << 8) | cAnimIdx);

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		const vector<UINT>& vecTarget = pObj->GetExclusiveClients();

		if(vecTarget.empty())
			GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(pObj->GetSceneID()));
		else
			GRoom.Unicast(pSendBuffer, vecTarget);
	}

	void EventManager::delete_object(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		GameObject* pObj = (GameObject*)_lParm;
		UINT iObjectID = pObj->GetObjectID();

		Scene* pScene = SceneManger::FindScene(pObj->GetSceneID());
		pScene->EraseObject(pObj->GetLayerType(), pObj);

		Protocol::S_DELETE pkt;

		pkt.set_scene_layer_deleteid((UCHAR)pObj->GetSceneID() << 24 | (UINT)pObj->GetLayerType() << 16 | iObjectID & 0xFFFF);
		pkt.set_pool_object(pObj->IsPoolObject());

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		const vector<UINT>& vecTarget = pObj->GetExclusiveClients();
		if (vecTarget.empty())
			GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(pObj->GetSceneID()));
		else
			GRoom.Unicast(pSendBuffer, vecTarget);

		delete pObj;
	}

	void EventManager::erase_object(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		GameObject* pObj = (GameObject*)_lParm;
		UINT iObjectID = pObj->GetObjectID();

		Scene* pScene = SceneManger::FindScene(pObj->GetSceneID());
		pScene->EraseObject(pObj->GetLayerType(), pObj);

		Protocol::S_DELETE pkt;

		pkt.set_scene_layer_deleteid((UCHAR)pObj->GetSceneID() << 24 | (UINT)pObj->GetLayerType() << 16 | iObjectID & 0xFFFF);

		pkt.set_pool_object(pObj->IsPoolObject());

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		const vector<UINT>& vecTarget = pObj->GetExclusiveClients();
		if (vecTarget.empty())
			GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(pObj->GetSceneID()));
		else
			GRoom.Unicast(pSendBuffer, vecTarget);
	}

	void EventManager::update_input(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iPlayerID = (UINT)_lParm;
		{
			WLock lock_guard(m_inputLock);
			int iPreIdx = 1 - m_iActiveInputIdx;
			vector<USHORT>& vecInput = m_vecInput[iPlayerID][iPreIdx];

			Input::Update_Key(iPlayerID, vecInput);

			vecInput.clear();
		}
	}

	void EventManager::create_player(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iPlayerID = (UINT)_lParm;
		UINT64 llPlayerEquip = (UINT64)_wParm;

		const UINT ValleySceneID = 4;
		
		Player* pPlayer = static_cast<Player*>(ObjectPoolManager::PopObject(L"Player"));
		pPlayer->m_iPlayerID = iPlayerID;
		pPlayer->SetObjectID(iPlayerID);
		pPlayer->Initialize();

		UCHAR cHairID = llPlayerEquip; UCHAR cEyeID = llPlayerEquip >> 8; UCHAR cHatID = llPlayerEquip >> 16; UCHAR cTopID = llPlayerEquip >> 24;
		UCHAR cBottomID = llPlayerEquip >> 32; UCHAR cShoesID = llPlayerEquip >> 40; UCHAR cWeaponID = llPlayerEquip >> 48;

	
		pPlayer->SetHair(cHairID); pPlayer->SetEye(cEyeID); pPlayer->SetEquip(eEquipType::Hat, cHatID); pPlayer->SetEquip(eEquipType::Top, cTopID);
		pPlayer->SetEquip(eEquipType::Bottom, cBottomID); pPlayer->SetEquip(eEquipType::Shoes, cShoesID); pPlayer->SetEquip(eEquipType::Weapon, cWeaponID);

		//기존의 플레이어들 알리기
		SceneManger::SendPlayersInfo(iPlayerID, ValleySceneID);
		SceneManger::AddPlayerScene(pPlayer, ValleySceneID);

		Protocol::S_ENTER pkt;
		pkt.set_player_id(iPlayerID);
		pkt.set_player_equip_ids(llPlayerEquip);
		pkt.set_success(true);

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.GetPersonByID(iPlayerID)->Send(pSendBuffer);

		//다른 플레이어들에게 알리기
		Protocol::S_PLAYER_CREATE otherPkt;
		Protocol::PlayerInfo tInfo = {};
		SceneManger::MakePlayerInfo(pPlayer, tInfo);
		*otherPkt.add_player_info() = tInfo;
		shared_ptr<SendBuffer> pSendOtherBuffer = ClientPacketHandler::MakeSendBuffer(otherPkt);

		GRoom.UnicastExcept(pSendOtherBuffer, SceneManger::GetPlayerIDs(ValleySceneID), iPlayerID);
	}

	void EventManager::delete_player(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iPlayerID = (UINT)_lParm;
		SceneManger::DeletePlayer(iPlayerID);
		SkillManager::ReleaseSkill(iPlayerID);

		Protocol::S_NEW_EXIT other_pkt;
		other_pkt.set_playerid(iPlayerID);
		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(other_pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(iPlayerID));
	}

	void EventManager::add_player_pool(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		GameObject* pObj = (GameObject*)_lParm;

		m_vecPlayer_Pool.push_back(pObj);
	}

	void EventManager::add_monster_pool(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		GameObject* pObj = (GameObject*)_lParm;

		m_vecMonster_Pool.push_back(pObj);
	}

	void EventManager::change_scene(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iPlayerID = (UINT)(_lParm);
		GameObject* pPlayer = SceneManger::FindPlayer(iPlayerID);
		if (pPlayer == nullptr)
			assert(nullptr);
	
		change_player_fsmstate((DWORD_PTR)pPlayer->GetScript<PlayerScript>()->m_pFSM, (DWORD_PTR)Player::ePlayerState::jump, 0, {});

		UINT iNextSceneID = (UINT)(_wParm);
		UINT iPrevSceneID = pPlayer->GetSceneID(); //swap 후 이름이 변경되지 않게 복사

		//다음씬 플레이어들 정보 보내기
		SceneManger::SendPlayersInfo(iPlayerID, iNextSceneID);

		//플레이어 옮기기
		SceneManger::SwapPlayer(pPlayer, iPrevSceneID, iNextSceneID);
		SceneManger::SendEnterScene(iPlayerID, iNextSceneID);
		


		//이전 맵에 플레이어 삭제됐다고 알리기
		Protocol::S_DELETE pkt;
		pkt.set_scene_layer_deleteid((UCHAR)iPrevSceneID << 24 | (UCHAR)eLayerType::Player << 16 | iPlayerID);
		pkt.set_pool_object(pPlayer->IsPoolObject());

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(iPrevSceneID));

	}

	void EventManager::start_scene(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iPlayerID = (UINT)(_lParm);
		UINT iSceneID = (UINT)(_wParm);

		SceneManger::SendEnterScene(iPlayerID, iSceneID);
	}


	void EventManager::chanage_state(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		GameObject* pObj = (GameObject*)_lParm;
		GameObject::eState eState = (GameObject::eState)_wParm;

		pObj->SetState(eState);
	}

	void EventManager::using_item(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iItemInfo = (UINT)_lParm;
	
		ItemManager::ExcuteItem(iItemInfo);
	}

	void EventManager::change_player_fsmstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		PlayerFSM* pFSM = (PlayerFSM*)_lParm;
		Player::ePlayerState eState = (Player::ePlayerState)_wParm;
		pFSM->ChangeState(eState);
	}

	void EventManager::change_player_skillstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iPlayerID = (UINT)_lParm;
		Player::ePlayerSkill _ePlayerSkill = (Player::ePlayerSkill)_wParm;

		SkillManager::SetActiveSkill(iPlayerID, _ePlayerSkill);
	}

	void EventManager::change_player_equip(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iPlayerInfo = (UINT)_lParm;
		UINT iPlayerItem = (UINT)_wParm;

		UCHAR cSceneID = (iPlayerInfo >> 24) & 0xFF;
		UCHAR cLayer = (iPlayerInfo >> 16) & 0xFF;
		UCHAR cPlayerID = (iPlayerInfo >> 8) & 0xFF;
		UCHAR cEquipID = iPlayerInfo & 0xFF;
	
		GameObject* pObj = SceneManger::FindPlayer(cSceneID, cPlayerID);
		if (pObj)
		{
			Player* pPlayer = static_cast<Player*>(pObj);

			if(iPlayerItem == 0)
				pPlayer->DisableEquip((eEquipType)cEquipID);
			else
				pPlayer->SetEquip((eEquipType)cEquipID, iPlayerItem);

			Protocol::S_EQUIP pkt;
			pkt.set_scene_layer_playerid_equipid(iPlayerInfo);
			pkt.set_item_id(iPlayerItem);

			shared_ptr<SendBuffer> pBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
			const vector<UINT>& vecIDs = SceneManger::GetPlayerIDs(cSceneID);
			GRoom.UnicastExcept(pBuffer, vecIDs, cPlayerID);
		}
	}

	void EventManager::add_player_skillstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		SkillState* pSkillState = (SkillState*)_lParm;
		UINT iPlayerID = pSkillState->GetPlayer()->GetPlayerID();
		SkillManager::AddSkill(iPlayerID, pSkillState);
	}

	void EventManager::init_player_skill(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iPlayerID = (UINT)_lParm;
		PlayerSkill* pPlayerSkill = (PlayerSkill*)_wParm;

		SkillManager::InitSkill(iPlayerID, pPlayerSkill);
	}

	void EventManager::release_player_skill(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		UINT iPlayerID = (UINT)_lParm;

		SkillManager::ReleaseSkill(iPlayerID);
	}

	void EventManager::change_monster_fsmstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		MonsterFSM* pFSM = (MonsterFSM*)_lParm;
		Monster::eMonsterState eState = (Monster::eMonsterState)_wParm;
		pFSM->ChangeState(eState);
	}

	void EventManager::hitch_abnormal(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		GameObject* pObj = (GameObject*)_lParm;
		BattleManager::eAbnormalType eType = (BattleManager::eAbnormalType)_wParm;
		float fAccValue = _accParm;

		BattleManager::excute_abnormal(eType, pObj, fAccValue);
	}

	void EventManager::restore(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
	{
		GameObject* pObj = (GameObject*)_lParm;
		BattleManager::eAbnormalType eType = (BattleManager::eAbnormalType)_wParm;
		float fAccStat = (float)_accParm;

		BattleManager::restore(pObj, eType, fAccStat);
	}

	void EventManager::up_stat(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm, const OBJECT_DATA& _tObjData)
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
			vector<USHORT>& vecTarget = m_vecInput[_iPlayerID][m_iActiveInputIdx];

			vecTarget.reserve(vecTarget.size() + _vecInput.size());
			vecTarget.insert(vecTarget.end(), _vecInput.begin(), _vecInput.end());
		}
		eve.eEventType = EVENT_TYPE::UPDATE_INPUT;
		AddEvent(eve);
	}

	void EventManager::CreatePlayer(UINT _ID, UINT64 _llEquip)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_ID;
		eve.wParm = (DWORD_PTR)_llEquip;
		
		eve.eEventType = EVENT_TYPE::CREATE_PLAYER;
		AddEvent(eve);
	}

	void EventManager::DeletePlayer(UINT _ID)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_ID;

		eve.eEventType = EVENT_TYPE::DELETE_PLAYER;
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

	void EventManager::StartScene(UINT _iPlayerID, UINT _iSceneID)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iPlayerID;
		eve.wParm = (DWORD_PTR)_iSceneID;

		eve.eEventType = EVENT_TYPE::START_SCENE;
		AddEvent(eve);
	}

	void EventManager::ChanageScene(UINT _iPlayerID, UINT _iSceneID)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iPlayerID;
		eve.wParm = (LONG_PTR)_iSceneID;

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
	void EventManager::ChanagePlayerEquip(UINT _iPlayerInfo, UINT _iItemID)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iPlayerInfo;
		eve.wParm = (DWORD_PTR)_iItemID;

		eve.eEventType = EVENT_TYPE::CHANGE_PLAYER_EQUIP;

		AddEvent(eve);
	}

	void EventManager::UsingItem(UINT _iItemInfo)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iItemInfo;

		eve.eEventType = EVENT_TYPE::USING_ITEM;

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