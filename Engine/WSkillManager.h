#pragma once
#include "WPlayer.h"
#include "WInfo.h"
#include "WSkillState.h"

namespace W
{
	class PlayerSkill;
		
	//그냥 이벤트 매니저에서 후처리로 만들떄 순차적으로 만들기
	class SkillManager
	{
	public:
		static void Update();

		//후처리
		static void InitSkill(UINT _iPlayerID, PlayerSkill* _pSkill);
		static void ReleaseSkill(UINT _iPlayerID);

		static void AddSkill(UINT _iPlayerID, SkillState* _pSkill);
		
		static void SetActiveSkill(UINT _iPlayerID, Player::ePlayerSkill _eSkill);
		static void AccAttack(UINT _iPlayerID, Vector3 _vPosition);

	private:
		static void update_skill();
		static void set_max(UINT _iPlayerID, Player::ePlayerSkill _eTarget);
		static bool check_cooltime(UINT _iPlayerID, Player::ePlayerSkill _eTarget);

		static PlayerSkill* FindPlayerSkill(UINT _iPlayerID) { return m_vecSkill[_iPlayerID]; }
	private:
		
		//플레이어 ID당 전부 관리
		static std::vector<PlayerSkill*> m_vecSkill;
		static std::vector<std::map<Player::ePlayerSkill, tSkillTime>> m_vecSkillTime;

	};
}


