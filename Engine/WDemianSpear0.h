#pragma once
#include "WMonsterAttackObject.h"

namespace W
{
	class DemianSpear0 : public MonsterAttackObject
	{
	public:
		DemianSpear0();
		virtual ~DemianSpear0();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		//virtual void Render()override;

		DECLARE_CREATE_ID;
	private:
		void off();

	private:
		UINT m_iCallCount;
		tMonsterAttack m_tMonsterAttack;


	};

}

