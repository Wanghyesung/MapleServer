#pragma once
#include "WGameObject.h"

namespace W
{
	class Monster;
	class MonsterBackHP;

	class MonsterHP : public GameObject
	{
	public:
		MonsterHP(bool bBoss);
		virtual ~MonsterHP();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID

		UINT GetHPCount() { return m_iHPCount; }
		bool IsActive() { return m_bActive; }
		void ActiveOn() { m_bActive = true; }

		void SetOwner(Monster* _pMonster) { m_pOwner = _pMonster; }
		bool IsBoss() { return m_bBoss; }

		void DeleteHP();

	private:
		void update_hp();

	protected:
		MonsterBackHP* m_pHPBack;

		float m_fHP;
	private:
		UINT m_iHPCount;
		static UINT HP_COUNT;

		Monster* m_pOwner;
		bool m_bActive;
		bool m_bBoss;

		friend class MonsterManager;
	};
}


