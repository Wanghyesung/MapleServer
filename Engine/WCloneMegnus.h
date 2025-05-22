#pragma once
#include "WMonsterAttackObject.h"
namespace W
{
	class CloneMegnus : public MonsterAttackObject
	{
	public:
		CloneMegnus();
		virtual ~CloneMegnus();

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


