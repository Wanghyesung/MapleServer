#pragma once
#include "WUI.h"

namespace W
{
	class BossTex : public UI
	{
	public:
		BossTex();
		virtual ~BossTex();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		virtual void MouseOn();//UI���� ���콺 �ö���� �� 
		virtual void MouseLbtnDown();//UI�ȿ��� ������ ��
		virtual void MouseLbtnUp();//UI�ȿ��� ���� ��
		virtual void MouseLbtnClicked();//UI�ȿ��� ������ ���� ��

		void ChanageTex(UINT _iTexNum);

	private:
		

	};

}


