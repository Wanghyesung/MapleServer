#pragma once
#include "WGameObject.h"
#include "WPlayer.h"
#include "WPlayerTop.h"
#include "WPlayerBottom.h"
#include "WPlayerShoes.h"
namespace W
{	
	class PlayerTop;
	class PlayerBottom;
	class PlayerShoes;

	class PlayerBody : public GameObject
	{
	public:
		PlayerBody();
		virtual ~PlayerBody();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
		DECLARE_CREATE_ID;

		void SetPlayer(Player* _pPlayer) { m_pPlayer = _pPlayer; }
		Player* GetPlayer() { return m_pPlayer; }

		void SetEquipTop(const std::wstring& _strEquipName);
		void SetEquipBottom(const std::wstring& _strEquipName);
		void SetEquipShoes(const std::wstring& _strEquipName);

		const wstring& GetEquipTopName();
		const wstring& GetEquipBottomName();
		const wstring& GetEquipShoesName();

		UINT GetTopEquipID() { return m_pPlayerTop->GetItemID(); }
		UINT GetBottomEquipID() { return m_pPlayerBottom->GetItemID(); }
		UINT GetShoesEquipID() { return m_pPlayerShoes->GetItemID(); }

		void SetAnimationIndex();

		void SetStop(bool _bStop);
	private:
		Player* m_pPlayer;

		PlayerTop* m_pPlayerTop;
		PlayerBottom* m_pPlayerBottom;
		PlayerShoes* m_pPlayerShoes;

		std::wstring m_strCurAnim;
	};
}


