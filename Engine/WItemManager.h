#pragma once
#include "..\Engine_Source\Engine.h"

namespace W
{
	enum class eIconType
	{
		Equip,
		Item,
		Another,
		Install,
		Cash,
		SKill,
		End,
	};
	enum class eEquipType
	{
		Pandant,
		Hat,
		Top,
		Bottom,
		Shoes,
		Weapon,
		None,
	};

	enum class eAppearance
	{
		None,
		Hair,
		Eye,
		End,
	};

	struct tItemInfo
	{
		UINT iItemID;
		UINT iItemLevel;
		UINT iFunctionID;
		eIconType eIconType;
		string strItemName;

		tItemInfo() :iItemID(0), iItemLevel(0), eIconType(eIconType::End), strItemName("") {}	
	};

	UINT ChangeHair(UINT _iPlayerID);
	UINT ChangeEye(UINT _iPlayerID);

	class ItemManager
	{
	public:
		static void Initialize();
		static void Release();

		//밖에서 삭제를 하지 못하게 weak_ptr로 반환 (유니크, 쉐어드 다 밖에서 삭제가 가능 서버에서 실수로 삭제하면 안됨)
		static weak_ptr<tItemInfo> GetItemInfo(UINT _iItemID);
		static weak_ptr<tItemInfo> GetItemInfo(const wstring& _strName);
		static int GetItemID(const wstring& _strName);

		static void ExcuteItem(UINT _iItemInfo);
		static void SendResultItem(UINT _iItemInfo, UINT _iValue);

	private:
		static void init_function();
	private:
		static map<UINT, shared_ptr<tItemInfo>> m_mapItems;
		static map<wstring, UINT> m_mapItemName;

		//반환 : 결과값(클라 전달용) //인자 : 사용할 플레이어 ID
		static vector<std::function<UINT(UINT)>> m_vecItemEvent;
	};
}
