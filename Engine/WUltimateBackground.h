#pragma once
#include "WPlayerAttackObject.h"
namespace W
{
	class UltimateBackground : public PlayerAttackObject
	{
	public:
		UltimateBackground();
		virtual ~UltimateBackground();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID;

	private:
		void update_state();

	};
}


