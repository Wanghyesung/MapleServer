#pragma once
#include "WUI.h"
#include "WBossUI.h"
namespace W
{
	class BossEnterUI : public UI
 	{
	public:
		BossEnterUI();
		virtual ~BossEnterUI();
		
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		virtual void MouseOn();//UI���� ���콺 �ö���� �� 
		virtual void MouseLbtnDown();//UI�ȿ��� ������ ��
		virtual void MouseLbtnUp();//UI�ȿ��� ���� ��
		virtual void MouseLbtnClicked();//UI�ȿ��� ������ ���� ��

	private:
		void set_nextscene(BossUI::eBossUnber _eNumber);

	private:
		std::wstring m_strNextScene;

		bool m_bDown;

		friend class BossUI;
	};
}


