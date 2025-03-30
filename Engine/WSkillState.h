#pragma once
#include "WPlayer.h"
#include "WPlayerSkill.h"
#include "WInfo.h"

namespace W
{
	class Effect;
	class PlayerAttackObject;
	class SkillState
	{
	public:
		SkillState();
		~SkillState();

		virtual void Enter();
		virtual void Exit();

		virtual void Update();
		virtual void Initialize();

		void SetSkillType(Player::ePlayerSkill _eSkill) { m_eSkill = _eSkill; }
		Player::ePlayerSkill GetPlayerSkill() { return m_eSkill; }


		void SetStateName(const std::wstring& _strName) { m_strStateName = _strName; }
		const std::wstring& GetStateName() { return m_strStateName; }

		void SetOnwer(PlayerSkill* _pSkill) { m_pOwner = _pSkill; }
		PlayerSkill* GetSkill() { return m_pOwner; }

		const tSkillTime& GetCoolTime() { return m_tSKillTime; }
		void SetCoolTime(const tSkillTime& _tTime) { m_tSKillTime = _tTime; }

		Player* GetPlayer();

	protected:
		Effect* CreateEffet(std::wstring _strName, Vector2 _vLeftTop, Vector2 _vSize, 
			UINT _iColumnLength, UINT _iRowLength, Vector2 _vDivisionSize, Vector2 _vOffset, float _fDuration);
		void StartEffect(Effect* _pEffect);

		void AddDamage(PlayerAttackObject* _pAttackObject);


	private:
		std::wstring m_strStateName;
		Player::ePlayerSkill m_eSkill;

		PlayerSkill* m_pOwner;


		tSkillTime m_tSKillTime;
		float m_fAddDamage;
	};
}


