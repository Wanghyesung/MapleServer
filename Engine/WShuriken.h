#pragma once

#include "WPlayerAttackObject.h"
#include "WInfo.h"

namespace W
{
	class Shuriken : public PlayerAttackObject
	{
	public:
		Shuriken();
		virtual ~Shuriken();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;
	

		DECLARE_CREATE_ID;

		int GetDir() { return m_iDir; }
		void SetDir(int _iDir);

		virtual void Off()override;

	private:
		void update_state();

	private:
		int m_iDir;

		UINT m_iCount;
		static UINT COUNT;
	};
}


