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
		//virtual void Render()override;

		int GetDir() { return m_iDir; }
		void SetDir(int _iDir) { m_iDir = _iDir; }

		virtual void Off()override;

		DECLARE_CREATE_ID;
	private:
		int m_iDir;

		//UINT m_iCount;
		//static UINT COUNT
	};
}


