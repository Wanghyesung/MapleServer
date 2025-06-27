#pragma once
#include "WPlayerAttackObject.h"

namespace W
{
	class Aven : public PlayerAttackObject
	{
	public:
		Aven();
		virtual ~Aven();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		virtual void Off()override;

		DECLARE_CREATE_ID;

	private:
		void update_state();


		//UINT m_iCount;
		//static UINT COUNT
	};
}


