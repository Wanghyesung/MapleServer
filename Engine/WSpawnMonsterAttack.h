#pragma once
#include "WMonsterAttackObject.h"

namespace W
{
	class Monster;

	class SpawnMonsterAttack : public MonsterAttackObject
	{
	public:
		SpawnMonsterAttack();
		virtual ~SpawnMonsterAttack();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void Render()override;
		
		void SetVelocity(const Vector2& _vVelocity, const Vector3& _vOffset = Vector3::Zero);
		void SetTime(float _fTime) { m_fCreateTime = _fTime; }
		void SetAttackName(const std::wstring _strName) { m_strAttackName = _strName; }
		void SetCreateCount(int _iCount) { m_iCreateCount = _iCount; }
		void SetEndFrame(int _iFrame) { m_iEndFrame = _iFrame; }


	private:
		 void off();

	private:
		std::vector<Vector2> m_vecVelocity;
		std::vector<Vector3> m_vecOffset;
		//���� ������Ʈ���� ������ ���� �̸�
		std::wstring m_strAttackName;

		//���� �ֱ�
		float m_fCreateTime;
		float m_fCurCreateTime;

		//�� ���� �ε��� �������� ��
		int m_iCurIndex;
		int m_iCreateCount;
		int m_iEndFrame;

		//
		bool m_bEndEvent;
	};
}


