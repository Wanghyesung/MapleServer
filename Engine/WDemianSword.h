#pragma once
#include "WMonsterAttackObject.h"
namespace W
{
	class DemianSword : public MonsterAttackObject
	{
	public:
		DemianSword();
		virtual ~DemianSword();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void Render()override;

	private:
		void off();

		void start();
		void restart();

		void move();
		void attack();

		void set_target();
		bool check_position();

		void set_dir();
		void change_dir();
		void set_rotate();

		void check_HP();
	private:
		bool m_bEnd;
		bool m_bMove;
		bool m_bWait;

		//�������� ���� �� ��ٸ��� �ð�
		float m_fWaitTime;
		float m_fCurWaitTime;

		//Ÿ������ ���� ��
		float m_fAttackTime;
		float m_fCurAttackTime;
	
		//���� ���ϴ� �ð�
		float m_fChangeTime;
		float m_fCurChangeTime;

		//3�� ���� �� ����
		bool m_bTargetOn;
		bool m_bAttackOn;
		UINT m_iMoveCount;

		//���� ��ġ
		//���� �������� �� �����
		std::vector<Vector2> m_vecDir;
		Vector2 m_vActiveDir;

		Vector2 m_vTargetPos;
		Vector2 m_vVelocity;

		UINT m_iDirIndex;

		tMonsterAttack m_tMonsterAttack;
		std::wstring m_strCurAnim;

		friend class SwordExplode;
	};
}


