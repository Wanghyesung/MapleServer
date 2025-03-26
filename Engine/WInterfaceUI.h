#pragma once
#include "WUI.h"
#include "WItemUI.h"
#include "WIconUI.h"
namespace W
{
	class Inventory;
	class ItemUI;

	class InterfaceUI : public UI
	{
	public:
		InterfaceUI();
		virtual ~InterfaceUI();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		virtual void MouseOn();//UI���� ���콺 �ö���� �� 
		virtual void MouseLbtnDown();//UI�ȿ��� ������ ��
		virtual void MouseLbtnUp();//UI�ȿ��� ���� ��
		virtual void MouseLbtnClicked();//UI�ȿ��� ������ ���� ��

		void InsertItem(IconUI* _pItem, std::wstring _strName);
		void DeleteItem(IconUI* _pItem) { m_mapItems.erase(_pItem->GetName()); }
		
		IconUI* FindItem(std::wstring _strName);

		void CheckItemPosition(IconUI* _pItem);
		

		bool ChangeItemPosition(IconUI* _pItem, Vector2 _vSetPosition);
		IconUI* FindItemOnPosition(UINT _iX, UINT _iY);
		void SetItemKeyCode(IconUI* _pItem, UINT _iX, UINT _iY);

		Vector2 GetStartPosition() { return m_vUIStartPosition; }
		Vector2 GetEndPosition() { return m_vUIEndPosition; }
		Vector2 GetDiffPosition() { return m_vUIDiffPosition; }


	private:
		//2.28 -3.21
		Vector2 m_vUIStartPosition;
		
		//3.3 -3.55
		Vector2 m_vUIEndPosition;
		
		// 0.255 //0.34
		Vector2 m_vUIDiffPosition;

		std::map<std::wstring, IconUI*> m_mapItems;

		std::unordered_map<UINT, eKeyCode> m_hashKeyCode;
	};

}

