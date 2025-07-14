#pragma once

#include "WGameObject.h"
#include "WPlayer.h"
#include "WPlayerHat.h"
namespace W
{
	class PlayerHat;
	class PlayerHair;
	class Eyes;
	class PlayerHead : public GameObject
	{
	public:
		PlayerHead();
		virtual ~PlayerHead();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		
		DECLARE_CREATE_ID;

		void SetPlayer(Player* _pPlayer) { m_pPlayer = _pPlayer; }
		Player* GetPlayer() { return m_pPlayer; }

		void SetAlert(bool _bAlert);
		void SetEquipHat(const std::wstring& _strEquipName);
		const wstring& GetHatEquipName();

		UINT GetHatEquipID() { return m_pPlayerHat->GetItemID(); }
		UINT GetHairID() { return m_pPlayerHair->GetHairID(); }
		UINT GetEyeID() { return m_pPlayerEyes->GetEyeID(); }


		void SetAnimationIndex();

		void SetStop(bool _bStop);

		void SetHair(UINT _iHairNum);
		void SetEye(UINT _iEyeNum);
	private:
		Player* m_pPlayer;

		Eyes* m_pPlayerEyes;
		PlayerHair* m_pPlayerHair;
		PlayerHat* m_pPlayerHat;

		std::wstring m_strCurAnim;
	};
}


