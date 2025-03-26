#pragma once
#include "WPlayerAttackObject.h"

namespace W
{
	class UltimateObject : public PlayerAttackObject
	{
	public:
		UltimateObject();
		virtual ~UltimateObject();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void Render()override;

		virtual void Off()override;

		int GetCurFrame() { return m_iCurFrame; }
		int GetAttackFrame() { return m_iAttackFrame; }
	private:
		void attack();
		
	private:
		float m_fCurResetTime;
		float m_fResetTime;

		int m_iCurFrame;
		int m_iAttackFrame;//���ݽ��� ������
		int m_iEndFrame;


		UINT m_iCreateCount;
	};
}


