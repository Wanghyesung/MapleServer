#pragma once
#include "WAbnormal.h"

namespace W
{
	class Temptation : public Abnormal
	{
	public:
		Temptation();
		virtual ~Temptation();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;
		DECLARE_CREATE_ID;

		virtual void Restore()override;
		
	private:

	};
}


