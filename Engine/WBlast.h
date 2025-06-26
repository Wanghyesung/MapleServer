#pragma once
#include "WPlayerAttackObject.h"

namespace W
{
	class Blast : public PlayerAttackObject
	{
	public:
		Blast();
		virtual ~Blast();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void UpdatePacket()override;

		virtual void Off()override;

		void SetDir(int _iDir);

		DECLARE_CREATE_ID;

	private:
		void update_state();

	private:
		int m_iDir;

		
	};
}


