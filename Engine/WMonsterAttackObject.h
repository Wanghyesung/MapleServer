#pragma once
#include "..\Engine_Source\WGameObject.h"
#include "..\Engine_Source\WInfo.h"
#include "WEventManager.h"

namespace W
{
	class Monster;
	class MonsterAttackObject : public GameObject
	{
	public:
		MonsterAttackObject();
		virtual ~MonsterAttackObject();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		//virtual void Render()override;

		DECLARE_CREATE_ID
	public:
		void SetOnwer(Monster* _pOnwer) { m_pOwner = _pOnwer; }

		void SetMonsterAttack(const tMonsterAttack& _tAttackInfo);

		//몬스터로 바꾸기
		Monster* GetOwner() { return m_pOwner; }
		

	private:
		Monster* m_pOwner;
	};
}

