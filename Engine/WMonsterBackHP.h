#pragma once
#include "WGameObject.h"

namespace W
{
	class MonsterHP;

	class MonsterBackHP : public GameObject
	{
	public:
		MonsterBackHP(bool _bBoss);
		virtual ~MonsterBackHP();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
		DECLARE_CREATE_ID
		void SetHP(MonsterHP* _pHP) { m_pHP = _pHP; }
	private:
		MonsterHP* m_pHP;
	};
}


