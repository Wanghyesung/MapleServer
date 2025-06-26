#pragma once
#include "WPlayerAttackObject.h"
namespace W
{
	class UltimateShuriken : public PlayerAttackObject
	{
	public:
		//������ ������ �̹��� �ѹ�
		UltimateShuriken();
		virtual ~UltimateShuriken();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void UpdatePacket()override;

		void SetStartPos(Vector2 _vPos);
		void SetTarget(GameObject* _pTarget) { m_pTarget = _pTarget; }

		DECLARE_CREATE_ID;

	private:
		void update_state();

	private:
		Vector2 m_vDir;
		GameObject* m_pTarget;

	};
}


