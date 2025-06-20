#pragma once
#include "WMonster.h"
namespace W
{
	class Horntail : public Monster
	{
	public:
		DECLARE_CREATE_ID;

		enum class eHorntailChild
		{
			HorntailHeadA,
			HorntailHeadB,
			HorntailHeadC,
			HorntailLeftHand,
			HorntailRightHand,
			HorntailLeg,
			HorntailTail,
			HorntailWing,
		};
		Horntail();
		virtual ~Horntail();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		void Start();

	private:
		void update_state();

		void create_child();
		void delete_child();

		void up_attack();
		void up_defense();
		void heal();
		void dead();

		void update_HP();

	private:
		bool m_bStart;
		
		int m_iDeadCount;

		std::vector<Monster*> m_vecMonster;
	};
}


