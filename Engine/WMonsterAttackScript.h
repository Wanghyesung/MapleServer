#pragma once

#include "WScript.h"
#include "WBattleManager.h"

namespace W
{
	class MonsterAttackScript : public Script
	{
	public:
		MonsterAttackScript();
		virtual ~MonsterAttackScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* other);
		virtual void OnCollisionStay(Collider2D* other);
		virtual void OnCollisionExit(Collider2D* other);

		const tAttackInfo& GetAttackInfo() { return m_tAttackInfo; }
		void SetAttackInfo(const tAttackInfo& _tAttackInfo) { m_tAttackInfo = _tAttackInfo; }

		void SeteAbnormalType(BattleManager::eAbnormalType _eType) { m_eAbnorType = _eType; }

		bool IsPlayerEnter() { return m_bPlayerEnter; }

		bool IsEnter() { return m_bEnter; }
		void SetEnter(bool _bEnter) { m_bEnter = _bEnter; }

		void InitStack() { m_iAbnormalStack = 1; }
	private:
		tAttackInfo m_tAttackInfo;

		BattleManager::eAbnormalType m_eAbnorType;
		float m_fAccValue;//������ ���³� , �ɷ�ġ ���϶� ���

		bool m_bEnter;
		bool m_bPlayerEnter;

		UINT m_iAbnormalStack;
	};

}

