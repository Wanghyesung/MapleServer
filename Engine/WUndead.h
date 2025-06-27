#pragma once
#include "WAbnormal.h"

namespace W
{
	class Undead :public Abnormal
	{
	public:
		Undead();
		virtual ~Undead();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		virtual void Restore()override;

		DECLARE_CREATE_ID;

	private:
	};
}


