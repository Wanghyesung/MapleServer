#pragma once
#include "WGameObject.h"
#include "WPlayer.h"
namespace W
{
	class PlayerWeapon;
	class PlayerArm : public GameObject
	{
	public:
		PlayerArm();
		virtual ~PlayerArm();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
		DECLARE_CREATE_ID;

		void SetPlayer(Player* _pPlayer) { m_pPlayer = _pPlayer; }
		Player* GetPlayer() { return m_pPlayer; }

		void SetEquipWeapon(const std::wstring& _strEquipName);

		void SetAnimationIndex();

		void SetStop(bool _bStop);


	private:
		Player* m_pPlayer;

		PlayerWeapon* m_pPlayerWeapon;
		std::wstring m_strCurAnim;
	};
}


