#pragma once
#include "WGameObject.h"
#include "WPlayer.h"
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
	
		void SetPlayer(Player* _pPlayer) { m_pPlayer = _pPlayer; }
		Player* GetPlayer() { return m_pPlayer; }

		void SetEquipTop(const std::wstring& _strEquipName);
		void SetEquipBottom(const std::wstring& _strEquipName);
		void SetEquipShoes(const std::wstring& _strEquipName);

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


