#pragma once
#include "WMonsterAttackObject.h"

namespace W
{
	class MiniBeanAttack : public MonsterAttackObject
	{
	public:
		MiniBeanAttack();
		virtual ~MiniBeanAttack();
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

		float m_fDeleteTIme;
		float m_fCurTime;
	};
}

