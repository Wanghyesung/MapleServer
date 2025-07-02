#pragma once
#include "WInput.h"
namespace W
{
	class InputBackground;
	class Player;
	class InputObject : public GameObject
	{
	public:
		InputObject(eKeyCode _eKeyCode);
		virtual ~InputObject();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID;

		void SetClear(bool _bClear);
		void SetOwner(InputBackground* _pOwner) { m_pOwner = _pOwner; }
		void SetIndex(UINT _iIndex) { m_iIndex = _iIndex; }
		UINT GetIndex() { return m_iIndex; }

		void SetTarget(Player* _pPlayer);

		static wstring GetWDir(eKeyCode _eKeyCode);
	private:
		void check();
		void failed();
		void clear();

	private:
		Player* m_pTarget;

		eKeyCode m_eKeyCode;
		InputBackground* m_pOwner;
		UINT m_iIndex;

		bool m_bActive;
		bool m_bRender;
		bool m_bPrevRender;
	};
}


