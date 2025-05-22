#pragma once
#include "WMonsterAttackObject.h"

namespace W
{
	class MegnusStone : public MonsterAttackObject
	{
	public:
		MegnusStone();
		virtual ~MegnusStone();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
		DECLARE_CREATE_ID;

	private:
		void move();
		void start();
		void off();
	private:
		tMonsterAttack m_tMonsterAttack;
		std::wstring m_strCurAnim;

		bool m_bMove;
		bool m_bEnd;
	};
}


