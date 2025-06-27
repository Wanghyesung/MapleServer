#pragma once
#include "WPlayerAttackObject.h"
namespace W
{
	class Wind : public PlayerAttackObject
	{
	public:
		Wind();
		virtual ~Wind();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID;

		void SetStop() { m_bStop = true; }
		void SetFinish() { m_bFinish = true; }

		virtual void Off()override;

	private:
		void update_state();

	private:

		float m_fResetTime;
		float m_fCurResetTime;

		float m_fFinishTime;
		float m_fCurFinishTime;

		float m_fStopTime;
		float m_fCurStopTime;

		bool m_bStop;
		bool m_bFinish;

		std::wstring m_strCurAnim;
	};
}


