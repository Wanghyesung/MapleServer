#pragma once

#include "WMonsterAttackObject.h"

namespace W
{
	class Fire : public MonsterAttackObject
	{
	public:
		Fire();
		virtual ~Fire();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
		DECLARE_CREATE_ID;
	private:
		void off();

	private:
		UINT m_iCallCount;
		tMonsterAttack m_tMonsterAttack;

	};
}


