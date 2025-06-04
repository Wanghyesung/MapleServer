#pragma once
#include "..\Engine\WEventManager.h"
#include "WPlayerFSM.h"
#include "WMonsterFSM.h"
#include "WBattleManager.h"
namespace W
{
	enum class EVENT_TYPE
	{
		CREATE_PLAYER,
		DELETE_PLAYER,
		UPDATE_INPUT,
		CREATE_OBJECT,
		DELET_OBJECT,
		ERASE_OBJECT,
		CHANGE_STATE,
		CHANGE_PLAYER_STATE,
		CHANGE_PLAYER_SKILL,
		ADD_PLAYER_SKILL,
		INIT_PLAYER_SKILL,
		RELEASE_PLAYER_SKILL,
		CHANGE_MONSTER_STATE,
		ADD_PLAYER_POOL,
		ADD_MONSTER_POOL,
		CHANGE_PLAYER_SCENE,
		HITCH_ABNORMAL,
		UP_STAT,
		RESTORE,
		END,
	};

	struct tEvent
	{
		DWORD_PTR wParm;
		DWORD_PTR lParm;
		LONG_PTR accParm;//추가 인자
		EVENT_TYPE eEventType;
	};

	
	class EventManager
	{
	public:
		
		static void Update();
		static void Initialize();
		static void AddEvent(const tEvent& _tEve);
		
		static void CreateObject(GameObject* _pObj, eLayerType _eLayer);
		static void DeleteObject(GameObject* _pObj);
		static void EraseObject(GameObject* _pObj);

		static void Update_Input(UINT _iPlayerID, const vector<USHORT>& _vecInput);
		static void CreatePlayer(UINT _ID);

		static void AddPlayerPool (GameObject* _pObj);
		static void AddMonsterPool(GameObject* _pObj);

		static void ChanageState(GameObject* _pObj, GameObject::eState _eState);

		static void ChangePlayerFSMState(PlayerFSM* _pFSM, Player::ePlayerState _ePlayerState);
		static void ChangePlayerSkillState(UINT _iPlayerID, Player::ePlayerSkill _ePlayerSkill);

		static void AddPlayerSkillState(class SkillState* _pSkillState);
		static void InitPlayerSkill(UINT _iPlayerID, class PlayerSkill* _pPlayerSkill);
		static void ReleasePlayerSkill(UINT _iPlayerID);

		//static void ChangePlayerScene(Player*)
		static void ChangeMonsterFSMState(MonsterFSM* _pFSM, Monster::eMonsterState _eMonsterState);
		static void HitchAbnormal(GameObject* _pObj, BattleManager::eAbnormalType _eType, float _fAccStat = 0.f);
		static void Restore(GameObject* _pObj, BattleManager::eAbnormalType _eType, float _fAccStat = 0.f);
		static void UpStat(GameObject* _pObj, BattleManager::eUpStatType _eType, float _fAccStat);

	private:
		static void excute(const tEvent& _tEve);
		static void pool_excute();

		static void create_object(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void delete_object(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void erase_object(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);

		static void update_input(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void create_player(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);

		static void add_player_pool(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void add_monster_pool(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);

		static void change_player_scene(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);

		static void chanage_state(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);

		static void change_player_fsmstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void change_player_skillstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);

		static void add_player_skillstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void init_player_skill(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void release_player_skill(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);

		static void change_monster_fsmstate(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void hitch_abnormal(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void restore(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);
		static void up_stat(DWORD_PTR _lParm, DWORD_PTR _wParm, LONG_PTR _accParm);

	private:
		static std::function<void(DWORD_PTR, DWORD_PTR, LONG_PTR)> m_arrFunction[(UINT)EVENT_TYPE::END];

		//만약 이벤트처리 벡터에서 lock을 걸면 새로들어오는 이벤트(패킷), 내가지금 처리해야할 이벤트 등 처리할 일이 많아짐
		//때문에 더블버퍼링으로 지금들어온 패킷은 다음 이벤트배열에 넣고 현재 이벤트들만 Engine에서 처리할 수 있게 만듬
		static std::vector<tEvent> m_vecEvent[2];
		static std::vector<GameObject*> m_vecPlayer_Pool;
		static std::vector<GameObject*> m_vecMonster_Pool;
		static std::vector<USHORT> m_vecInput[5][2];

		static RWLock m_lock;
		static RWLock m_inputLock;

		static atomic<int> m_iActiveInputIdx;
		static atomic<int> m_iActiveIdx;
	};
}


