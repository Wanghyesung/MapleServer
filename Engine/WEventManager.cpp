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
namespace W
{
	std::vector<tEvent> EventManager::m_vecEvent[2] = {};
	std::vector<GameObject*> EventManager::m_vecPlayer_Pool = {};
	std::vector<GameObject*> EventManager::m_vecMonster_Pool = {};
	std::vector<USHORT> EventManager::m_vecInput[5] = {};

	RWLock EventManager::m_lock = {};
	atomic<int> EventManager::m_iActiveIdx = 1;

#define ObjectPoolPosition 2000.f
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
		m_vecPlayer_Pool.clear();

		for (int i = 0; i < m_vecMonster_Pool.size(); ++i)
		{
			ObjectPoolManager::AddObjectPool(m_vecMonster_Pool[i]->GetName(), m_vecMonster_Pool[i]);
			//erase를 여기서 시키기
			SceneManger::Erase(m_vecMonster_Pool[i]);
		}
		m_vecMonster_Pool.clear();
	}

	void EventManager::excute(const tEvent& _tEve)
	{
		switch (_tEve.eEventType)
		{
		case EVENT_TYPE::CREATE_PLAYER:
		{
			UINT iPlayerID = (UINT)_tEve.lParm;

			Player* pPlayer = new Player();
			pPlayer->m_iPlayerID = iPlayerID;
			pPlayer->SetObjectID(iPlayerID);
			pPlayer->Initialize();
			SceneManger::AddPlayerScene(pPlayer, L"Valley");

		}
		case EVENT_TYPE::UPDATE_INPUT:
		{
			UINT iPlayerID = (UINT)_tEve.lParm;
			
			Input::Update_Key(iPlayerID, m_vecInput[iPlayerID]);
		}
		break;
		case EVENT_TYPE::CREATE_OBJECT:
		{
			GameObject* pObj = (GameObject*)_tEve.lParm;
			eLayerType eLyaer = (eLayerType)_tEve.wParm;

			SceneManger::AddGameObject(pObj->GetSceneName(), eLyaer, pObj);
		}
		break;

		case EVENT_TYPE::DELET_OBJECT:
		{
			GameObject* pObj = (GameObject*)_tEve.lParm;
			
			Scene* pScene = SceneManger::FindScene(pObj->GetSceneName());
			pScene->EraseObject(pObj->GetLayerType(),pObj);

			delete pObj;
		}
		break;

		case EVENT_TYPE::CHANGE_PLAYER_STATE:
		{
			PlayerFSM* pFSM = (PlayerFSM*)_tEve.lParm;
			Player::ePlayerState eState = (Player::ePlayerState)_tEve.wParm;
			pFSM->ChangeState(eState);
		}
		break;
		case EVENT_TYPE::CHANGE_PLAYER_SKILL:
		{
			UINT iPlayerID = (UINT)_tEve.lParm;
			Player::ePlayerSkill _ePlayerSkill = (Player::ePlayerSkill)_tEve.wParm;
			
			SkillManager::SetActiveSkill(iPlayerID, _ePlayerSkill);
		}
		break;

		case EVENT_TYPE::ADD_PLAYER_SKILL:
		{
			SkillState* pSkillState = (SkillState*)_tEve.lParm;
			UINT iPlayerID = pSkillState->GetPlayer()->GetPlayerID();
			SkillManager::AddSkill(iPlayerID,pSkillState);
		}
		break;

		case EVENT_TYPE::INIT_PLAYER_SKILL:
		{
			UINT iPlayerID = (UINT)_tEve.lParm;
			PlayerSkill* pPlayerSkill = (PlayerSkill*)_tEve.wParm;

			SkillManager::InitSkill(iPlayerID, pPlayerSkill);
		}
		break;

		case EVENT_TYPE::RELEASE_PLAYER_SKILL:
		{
			UINT iPlayerID = (UINT)_tEve.lParm;
		
			SkillManager::ReleaseSkill(iPlayerID);
		}
		break;

		case EVENT_TYPE::CHANGE_MONSTER_STATE:
		{
			MonsterFSM* pFSM = (MonsterFSM*)_tEve.lParm;
			Monster::eMonsterState eState = (Monster::eMonsterState)_tEve.wParm;
			pFSM->ChangeState(eState);
		}
		break;
		
		case EVENT_TYPE::ADD_PLAYER_POOL:
		{
			GameObject* pObj = (GameObject*)_tEve.lParm;
			
			m_vecPlayer_Pool.push_back(pObj);
		}
		break;

		case EVENT_TYPE::ADD_MONSTER_POOL:
		{
			GameObject* pObj = (GameObject*)_tEve.lParm;

			m_vecMonster_Pool.push_back(pObj);
		}
		break;

		case EVENT_TYPE::HITCH_ABNORMAL:
		{
			GameObject* pObj = (GameObject*)_tEve.lParm;
			BattleManager::eAbnormalType eType = (BattleManager::eAbnormalType)_tEve.wParm;
			switch (eType)
			{
			case W::BattleManager::eAbnormalType::SealPotion:
				BattleManager::seal_potion(pObj);
				break;
			case W::BattleManager::eAbnormalType::SealSkill:
				BattleManager::seal_skill(pObj);
				break;
			case W::BattleManager::eAbnormalType::temptation:
				BattleManager::temptation(pObj);
				break;
			case W::BattleManager::eAbnormalType::Undead:
				BattleManager::undead(pObj);
				break;
			case W::BattleManager::eAbnormalType::Faint:
				BattleManager::faint(pObj);
				break;
			case W::BattleManager::eAbnormalType::Stop:
				BattleManager::stop(pObj);
				break;
			case W::BattleManager::eAbnormalType::Dark:
				BattleManager::dark(pObj);
				break;
			case W::BattleManager::eAbnormalType::Confusion:
				BattleManager::confusion(pObj);
				break;
			case W::BattleManager::eAbnormalType::Slow:
				BattleManager::debuff_slow(pObj, _tEve.accParm);//능력치 깍는 효과는 accparm추가
				break;
			case W::BattleManager::eAbnormalType::Variation:
				BattleManager::variation(pObj);
				break;
			case W::BattleManager::eAbnormalType::DemianStop:
				BattleManager::demianstop(pObj);
				break;
			case W::BattleManager::eAbnormalType::InputKey:
				BattleManager::inputkey(pObj);
				break;
			case W::BattleManager::eAbnormalType::Stigma:
				BattleManager::stigma(pObj);
				break;
			}
		}
		break;

		case EVENT_TYPE::UP_STAT:
		{
			GameObject* pObj = (GameObject*)_tEve.lParm;
			BattleManager::eUpStatType eType = (BattleManager::eUpStatType)_tEve.wParm;
			float fAccStat = (float)_tEve.accParm;
			switch (eType)
			{
			case W::BattleManager::eUpStatType::Attack:
				BattleManager::buff_attack(pObj, fAccStat);
				break;
			case W::BattleManager::eUpStatType::Speed:
				BattleManager::buff_speed(pObj, fAccStat);
				break;
			case W::BattleManager::eUpStatType::Defense:
				BattleManager::buff_defense(pObj, fAccStat);
				break;
			case W::BattleManager::eUpStatType::Heal:
				BattleManager::buff_heal(pObj, fAccStat);
				break;
			case W::BattleManager::eUpStatType::Ignore:
				BattleManager::buff_ignore(pObj, fAccStat);
				break;
			case W::BattleManager::eUpStatType::ReflexAttack:
				BattleManager::buff_reflex(pObj, fAccStat);
				break;
			//player
			case W::BattleManager::eUpStatType::AccHP:
				BattleManager::buff_HP(pObj,fAccStat);
				break;
			case W::BattleManager::eUpStatType::AccMP:
				BattleManager::buff_MP(pObj,fAccStat);
				break;
			}
		}
		break;

		case EVENT_TYPE::RESTORE:
		{
			GameObject* pObj = (GameObject*)_tEve.lParm;
			BattleManager::eAbnormalType eType = (BattleManager::eAbnormalType)_tEve.wParm;
			float fAccStat = (float)_tEve.accParm;
			BattleManager::restore(pObj,eType,fAccStat);
		}
		break;
		}
	}

	
	

	/*void EventManager::AddPlayer(GameObject* _pObj)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pObj;
		
		eve.eEventType = EVENT_TYPE::CREATE_PLAYER;
		AddEvent(eve);
	}

	void EventManager::DeletePlayer(GameObject* _pObj)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pObj;

		eve.eEventType = EVENT_TYPE::DELETE_PLAYER;
		AddEvent(eve);
	}*/

	void EventManager::DeleteObject(GameObject* _pObj)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pObj;

		eve.eEventType = EVENT_TYPE::DELET_OBJECT;
		AddEvent(eve);
	}

	void EventManager::Update_Input(UINT _iPlayerID, const std::vector<USHORT>& _vecInput)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_iPlayerID;
		
		m_vecInput[_iPlayerID] = _vecInput;

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

	void EventManager::ChangePlayerFSMState(PlayerFSM* _pFSM, Player::ePlayerState _ePlayerState)
	{
		tEvent eve = {};
		eve.lParm = (DWORD_PTR)_pFSM;//보낼 주소
		eve.wParm = (DWORD_PTR)_ePlayerState;//다음 상태값
		eve.eEventType = EVENT_TYPE::CHANGE_PLAYER_STATE;

		AddEvent(eve);
	}
	void EventManager::ChangePlayerSkillState(Player* _pObj, Player::ePlayerSkill _ePlayerSkill)
	{
		tEvent eve = {};
		
		eve.lParm = (DWORD_PTR)_pObj->GetPlayerID();
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