#pragma once
#include "WMonsterAttackObject.h"

namespace W
{
	class Spear : public MonsterAttackObject
	{
	public:
		Spear();
		virtual ~Spear();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;
		
		void SetTarget(GameObject* _pTarget) { m_pTarget = _pTarget; }
		DECLARE_CREATE_ID;

	private:
		void off();

	private:
		UINT m_iCallCount;
		tMonsterAttack m_tMonsterAttack;

		GameObject* m_pTarget;
	};
}


