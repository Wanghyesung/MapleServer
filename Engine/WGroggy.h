#pragma once
#include "WAbnormal.h"

namespace W
{
	class Groggy : public Abnormal
	{
	public:
		Groggy();
		virtual ~Groggy();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		virtual void Restore()override;

		DECLARE_CREATE_ID;
	};
}


