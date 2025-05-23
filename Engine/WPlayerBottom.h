#pragma once
#include "WGameObject.h"
namespace W
{
	class PlayerBody;
	class Equip;

	class PlayerBottom : public GameObject
	{
	public:
		PlayerBottom();
		virtual ~PlayerBottom();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;

		DECLARE_CREATE_ID;

		void SetPlayerBody(PlayerBody* _pPlayerBody) { m_pPlayerBody = _pPlayerBody; }

		void SetPlayerEquip(const std::wstring& _strEquipName);

		const std::wstring& GetCurAnimName() { return m_strCurAnim; }

		void SetStop(bool _bStop) { GetComponent<Animator>()->Stop(_bStop); }
	private:
		PlayerBody* m_pPlayerBody;

		std::wstring m_strCurAnim;
		std::wstring m_strCurEquip;

	};
}


