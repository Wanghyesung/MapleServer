#pragma once
#include "WAbnormal.h"

namespace W
{
	class Variation : public Abnormal
	{
	public:
		Variation();
		virtual ~Variation();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
	private:
		void off();
	};
}


