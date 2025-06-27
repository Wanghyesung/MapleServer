#pragma once
#include "WMonster.h"

namespace W
{
	class Skelegon : public Monster
	{
	public:
		Skelegon();
		virtual ~Skelegon();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;
		
		DECLARE_CREATE_ID;

	private:
		void add_skill();

	private:
		void create_laser();

	};
}


