#pragma once

namespace W
{
	enum class eAttackType
	{
		Knockback,
		SuperKnockback,
		End,
	};

	struct tHitInfo
	{
		//���� ���� Ÿ��
		eAttackType eAttType;

		float fHitDir; //�´� ����
		float fHitRcnt;//������ ���� ��
		float fHitUpperRcnt; //������ ����� ��
		float fHitRigidityTime; //���� �ð�
	};

	struct tAttackInfo
	{
		tAttackInfo():
			fAttack(5.f),
			fMaxAttack(5.f),
			fDex(10.f),
			iDamageCount(0),
			eAttType(eAttackType::Knockback)
		{}

		//�⺻ ���ݷ�, dex, ������
		float fAttack;
		float fMaxAttack;
		float fDex;
		float fAttackDamage; //������

		float fAttRcnt;//������ ���� �� X
		float fAttUpperRcnt; //������ ����� �� Y
		float fAttRigidityTime; //���� �ð�

		//��Ʈ ������ ����
		UINT iDamageCount;
		eAttackType eAttType;
	};

	struct tObjectInfo
	{
		tObjectInfo()
		{
			fHP = 100.f;
			fMaxHP = fHP;
			fMP = 100.f;
			fMaxMP = fMP;
			fDefense = 1.f;
		}

		float fMaxHP;
		float fHP;
		float fMaxMP;
		float fMP;
		float fEXP;
		float fSpeed;
		float fJumpValue;
		float fDefense;
	};

	struct tSkillTime
	{
		float fCurTime;
		float fCoolTime;
		float fActiveTime;
		float fCurActiveTime;
	};

	

	struct tMonsterAttack
	{
		tMonsterAttack():
			vOffset(math::Vector2::Zero)
		{
			bSkill = false;
			tTime.fCoolTime = 4;
			tTime.fCurTime = 0;

			tAttackInfo.fAttRigidityTime = 0.f;
			tAttackInfo.fAttRcnt = 1.2f;
			tAttackInfo.fAttUpperRcnt = 1.5f;
		}
		tSkillTime tTime;

		tAttackInfo tAttackInfo;

		std::function<void()> pFunction;

		math::Vector2 vScale;
		math::Vector2 vOffset;
		math::Vector3 vPosition;
		math::Vector3 vRoatate;

		//���ݽ��� ������ ������ ������
		UINT iStartFrame;
		UINT iEndFrame;

		//��ų(ex : ��ȯ ������Ʈ)
		bool bSkill;
	};
}