#pragma once
#include "WMonsterHP.h"

namespace W
{
	class Monster;
	class MonsterBackHP;

	class SharHP : public MonsterHP
	{
	public:
		SharHP();
		virtual ~SharHP();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID;

		void SetHPValue(float _fValue) { m_fHP = _fValue; }
	private:
		friend class MonsterManager;
	};

	
}


