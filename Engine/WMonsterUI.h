#pragma once
#include "WUI.h"
#include "WBossUI.h"
namespace W
{
	class MonsterUI : public UI
	{
	public:
		MonsterUI();
		virtual ~MonsterUI();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		virtual void MouseOn();//UI���� ���콺 �ö���� �� 
		virtual void MouseLbtnDown();//UI�ȿ��� ������ ��
		virtual void MouseLbtnUp();//UI�ȿ��� ���� ��
		virtual void MouseLbtnClicked();//UI�ȿ��� ������ ���� ��

		BossUI::eBossUnber GetBossNumber() { return m_eBossNumber; }
	private:
		void set_bossnumber(BossUI::eBossUnber _eNumber);
		void clear();
	private:
		static UINT COUNT;
		UINT m_iCount;

		bool m_bSelete;
		BossUI::eBossUnber m_eBossNumber;

		friend class BossUI;
	};
}


