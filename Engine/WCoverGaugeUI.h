#pragma once
#include "WUI.h"
namespace W
{
	class CoverGaugeUI : public UI
	{
	public:
		CoverGaugeUI();
		virtual ~CoverGaugeUI();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void Render()override;

		virtual void MouseOn()override;
		virtual void MouseLbtnDown()override;
		virtual void MouseLbtnUp()override;
		virtual void MouseLbtnClicked()override;


	};
}


