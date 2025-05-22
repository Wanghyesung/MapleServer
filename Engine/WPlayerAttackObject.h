#pragma once
#include "WGameObject.h"

namespace W
{
	class Player;
	class Effect;
	class PlayerAttackObject : public GameObject
	{
	public:
		PlayerAttackObject();
		virtual ~PlayerAttackObject();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
	

		DECLARE_CREATE_ID;

		Player* GetPlayer() { return m_pPlayer; }
		void SetPlayer(Player* _pPlayer){ m_pPlayer = _pPlayer;}

		virtual void Off();
		void PushObjectPool();

		
	private:
		Player* m_pPlayer;
	};
}


