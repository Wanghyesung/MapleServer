#pragma once
#include "WGameObject.h"

namespace W
{
	class Player;
	class InputBackground : public GameObject
	{
	public:
		InputBackground();
		virtual ~InputBackground();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		//virtual void Render()override;

		void SetOwner(GameObject* _pObj) { m_pOwner = _pObj; }

		void Failed();
		void Next();
		UINT GetCurIndex() { return m_iCurIndex; }
		void SetTarget(Player* _pPlayer) { m_pTarget = _pPlayer; }

	private:
		void next_button();

	private:
		//�����̳� ���н� groggy������Ʈ�� time ����
		GameObject* m_pOwner;

		Player* m_pTarget;
		UINT m_iCurIndex;
		std::vector<class InputObject*> m_vecInput;
		bool m_bNext;
		
	};
}


