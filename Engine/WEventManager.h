#pragma once
#include "Engine.h"
#include "WPlayerFSM.h"
#include "WMonsterFSM.h"
#include "WBattleManager.h"
namespace W
{
	enum class EVENT_TYPE
	{
		/*CREATE_PLAYER,
		DELETE_PLAYER,*/
		CREATE_OBJECT,
		DELET_OBJECT,
		CHANGE_PLAYER_STATE,
		CHANGE_PLAYER_SKILL,
		ADD_PLAYER_SKILL,
		INIT_PLAYER_SKILL,
		RELEASE_PLAYER_SKILL,
		CHANGE_MONSTER_STATE,
		ADD_PLAYER_POOL,
		ADD_MONSTER_POOL,
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
		static void AddEvent(const tEvent& _tEve);
		
		static void CreateObject(GameObject* _pObj, eLayerType _eLayer);
		static void DeleteObject(GameObject* _pObj);

		//static void AddPlayer(GameObject* _pObj);
		//static void DeletePlayer(GameObject* _pObj);

		static void AddPlayerPool (GameObject* _pObj);
		static void AddMonsterPool(GameObject* _pObj);

		static void ChangePlayerFSMState(PlayerFSM* _pFSM, Player::ePlayerState _ePlayerState);
		static void ChangePlayerSkillState(Player* _pObj, Player::ePlayerSkill _ePlayerSkill);

		static void AddPlayerSkillState(class SkillState* _pSkillState);
		static void InitPlayerSkill(UINT _iPlayerID, class PlayerSkill* _pPlayerSkill);
		static void ReleasePlayerSkill(UINT _iPlayerID);

		static void ChangeMonsterFSMState(MonsterFSM* _pFSM, Monster::eMonsterState _eMonsterState);
		static void HitchAbnormal(GameObject* _pObj, BattleManager::eAbnormalType _eType, float _fAccStat = 0.f);
		static void Restore(GameObject* _pObj, BattleManager::eAbnormalType _eType, float _fAccStat = 0.f);
		static void UpStat(GameObject* _pObj, BattleManager::eUpStatType _eType, float _fAccStat);

	private:
		static void excute(const tEvent& _tEve);
		static void enter_excute();
		static void pool_excute();
	private:
		static std::vector<tEvent> m_vecEvent[2];
		static std::vector<GameObject*> m_vecPlayer_Pool;
		static std::vector<GameObject*> m_vecMonster_Pool;
		static RWLock m_lock;
		


		static int m_iActiveIdx;
	};
}


