#pragma once
#include "WAbnormal.h"
namespace W
{
	class Stop : public Abnormal
	{
	public:
		Stop();
		virtual ~Stop();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
		DECLARE_CREATE_ID;

		virtual void Restore()override;

	private:
	};
}


