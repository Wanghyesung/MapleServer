#pragma once
#include "WPlayerAttackObject.h"
namespace W
{
	class UltimateFog : public PlayerAttackObject
	{
	public:
		UltimateFog();
		virtual ~UltimateFog();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID;

	private:
		void update_state();

	};
}


