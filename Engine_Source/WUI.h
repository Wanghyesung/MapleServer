#pragma once
#include "WGameObject.h"
#include "WResources.h"
namespace W
{
	class UI : public GameObject
	{
	public:
		UI();
		UI(const UI& _pOrigin);
		virtual ~UI();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		virtual void MouseOn();//UI���� ���콺 �ö���� �� 
		virtual void MouseLbtnDown();//UI�ȿ��� ������ ��
		virtual void MouseLbtnUp();//UI�ȿ��� ���� ��
		virtual void MouseLbtnClicked();//UI�ȿ��� ������ ���� ��

		UI* GetParentUI() { return m_pParentUI; }
		void AddChildUI(UI* _pUI, bool _bMove = true);

		//�� ���Ϳ��� �����
		void DeleteChildUI(UI* _pUI);

		void MoveUI(UI* _pUI);
		void MoveToParent(Vector2 _vDiff);

		bool IsLBntDown() { return m_bLbntDown; }
		bool IsMosueOn() { return m_bMouseOn; }

		//private -> public
		void MouseOnCheck();

		const std::vector<UI*> GetChildUI() { return m_vecChildUI; }

	private:
		//�ڽ� UI ������Ʈ
		void ChildUpdate();
		void ChildRender();
		void ChildLateupdate();

		//void MoveToParent();

	private:
		//�� �ڽ� UI
		std::vector<UI*> m_vecChildUI;
		//�� �θ� UI
		UI* m_pParentUI;

		bool m_bMouseOn;
		bool m_bLbntDown;

		friend class UIManger;
	};

}

