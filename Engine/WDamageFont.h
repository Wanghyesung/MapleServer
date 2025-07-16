#pragma once
#include "WGameObject.h"

namespace W
{
	class DamageFont :public GameObject
	{
	public:
		DamageFont();
		virtual ~DamageFont();

		DECLARE_CREATE_ID;

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		void CheckDamage(UINT _iDamage);

	private:
		bool m_bActive;

		float m_fCurTime;
		float m_fDeleteTime;
		float m_fAlfhaColor;


		UINT m_iDamage;
		UINT m_iCount;
		static UINT FontCount;

		friend class BattleManager;
	};
}
