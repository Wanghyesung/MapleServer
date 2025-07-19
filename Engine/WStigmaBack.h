#pragma once
#include "WGameObject.h"

namespace W
{
	class StigmaCount;
	class StigmaBack : public GameObject
	{
	public:
		StigmaBack();
		virtual ~StigmaBack();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		DECLARE_CREATE_ID;

		void DeleteCount();
		void SetTarget(GameObject* _pObj) { m_pTarget = _pObj; }
	private:
		void set_count();

	private:
		GameObject* m_pTarget;
		StigmaCount* m_pCount;
		UINT m_iCount;
	};

}

