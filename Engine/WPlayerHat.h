#pragma once

#include "WGameObject.h"

namespace W
{
	class PlayerHead;
	class Equip;
	class PlayerHat : public GameObject
	{
	public:
		PlayerHat();
		virtual ~PlayerHat();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		

		DECLARE_CREATE_ID;

		void SetPlayerHead(PlayerHead* _pPlayerHead) { m_pPlayerHead = _pPlayerHead; }

		void SetPlayerEquip(const std::wstring& _strEquipName);

		const std::wstring& GetCurAnimName() { return m_strCurAnim; }
		const std::wstring& GetEquipName() { return m_strCurEquip; }

		void SetStop(bool _bStop) { GetComponent<Animator>()->Stop(_bStop); }

		UINT GetItemID() { return m_iItemID; }

	private:
		UINT m_iItemID;
		PlayerHead* m_pPlayerHead;

		std::wstring m_strCurAnim;
		std::wstring m_strCurEquip;


	};
}
	

