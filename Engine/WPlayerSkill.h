#pragma once

namespace W
{
	class SkillState;
	class PlayerSkill 
	{
	public:
		PlayerSkill();
		~PlayerSkill();

		void Update();
		void Initialize();
		void RegisterSkill();
		
		void SetPlayer(Player* _pPlayer) { m_pPlayer = _pPlayer; }
		Player* GetPlayer() { return m_pPlayer; }

		bool IsSkillOn() { return m_bSkillOn; }

		SkillState* FindSkillState(Player::ePlayerSkill _eSkill);
		void AccAttack(Vector3 _vPosition);
		void AddSkill(SkillState* _pSkill);

		const wstring& GetCurSkillName();
	private:	
		std::map<Player::ePlayerSkill, SkillState*> m_mapSkills;
		SkillState* m_pActiveSkill;

		Player* m_pPlayer;
		bool m_bSkillOn;

		friend class SkillManager;
	};
	
}


