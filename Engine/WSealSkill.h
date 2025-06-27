#pragma once
#include "WAbnormal.h"

namespace W
{
	class SealSkill : public Abnormal
	{
	public:
		SealSkill();
		virtual ~SealSkill();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID;

		virtual void Restore()override;
	private:
	};
}

