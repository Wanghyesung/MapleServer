#pragma once
#include "..\Engine_Source\WGameObject.h"

namespace W
{
	class Effect : public GameObject
	{
	public:
		Effect();
		virtual ~Effect();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID;

		void SetOwner(GameObject* _pOwner) { m_pOwner = _pOwner; }
		void CreateAnimation(Vector2 _vLeftTop, Vector2 _vSize, UINT _iColumnLength,
			UINT _iRowLength, Vector2 _vDivisionSize, Vector2 _vOffset, float _fDuration);

		void StartEffect(int _iDir);
		void SetPosition(Collider2D* _pCollider);
		void SetActive(bool _bActive) { m_bActive = _bActive; }
		bool IsActive() { return m_bActive; }

		void SetFunction(std::function<void()> _func, UINT _iStartIndex) 
		{
			m_pFunction = std::move(_func); 
			m_iStartIndex = _iStartIndex;
		}
	private:
		void off();
		
		std::function<void()> m_pFunction;

		void update_state();
	private:
		GameObject* m_pOwner;
		Vector2 m_vOffset;

		bool m_bActive;
		UINT m_iStartIndex;
		UINT m_iFuncCount;

		int m_iDir;
		
	};

}

