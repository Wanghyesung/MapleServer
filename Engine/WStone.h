#pragma once
#include "WGameObject.h"

namespace W
{
	class Stone : public GameObject
	{
	public:
		Stone();
		virtual ~Stone();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;
		DECLARE_CREATE_ID;

	private:
		void update_state();
	};
}


