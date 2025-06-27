#pragma once
#include "WAbnormal.h"

namespace W
{
	class Faint : public Abnormal
	{
	public:
		Faint();
		virtual ~Faint();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		virtual void Restore()override;
		
		DECLARE_CREATE_ID;
	private:
	};
}


