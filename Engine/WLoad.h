#pragma once
#include "WPlayerAttackObject.h"

namespace W
{
	class Load : public PlayerAttackObject
	{
	public:
		Load();
		virtual ~Load();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void UpdatePacket() override;

		DECLARE_CREATE_ID;

		virtual void Off()override;
		void Start();

	private:
		void update_state();
		void end();
		void create_shuriken();

		void init_skill();

	private:
		float m_fRespawnTime;
		float m_fCurRespawnTime;

		float m_fEndTime;
		float m_fCurEndTime;

		bool m_bEnd;
		

	};
}


