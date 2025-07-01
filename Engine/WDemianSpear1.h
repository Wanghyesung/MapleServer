#pragma once
#include "WMonsterAttackObject.h"

namespace W
{
	class DemianSpear1 : public MonsterAttackObject
	{
	public:
		DemianSpear1();
		virtual ~DemianSpear1();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID;
	private:
		void off();

	private:
		UINT m_iCallCount;
		tMonsterAttack m_tMonsterAttack;


	};

}

