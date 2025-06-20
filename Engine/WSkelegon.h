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
		//virtual void Render()override;

		DECLARE_CREATE_ID;

	private:
		void add_skill();

	private:
		void create_laser();

		void update_state();
		//std::shared_ptr<Texture> m_spAttackEffect;
	};
}


