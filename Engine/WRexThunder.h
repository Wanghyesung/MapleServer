#pragma once

#include "WMonsterAttackObject.h"

namespace W
{
	class RexThunder : public MonsterAttackObject
	{
	public:
		RexThunder();
		virtual ~RexThunder();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
		DECLARE_CREATE_ID;

	private:
		void off();

	private:
		UINT m_iCallCount;
		tMonsterAttack m_tMonsterAttack;

		GameObject* m_pTarget;
	};
}
