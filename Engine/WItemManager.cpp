#include "WItemManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "WPathManager.h"

namespace W
{
	using json = nlohmann::json;

	map<UINT, shared_ptr<tItemInfo>> ItemManager::m_mapItems= {};
	map<wstring, UINT> ItemManager::m_mapItemName = {};

	void ItemManager::Initialize()
	{
		//이거말고도 function 맵핑하기
		unordered_map<string, UINT> hashItem = 
		{
			{"Equip", (UINT)eIconType::Equip},
			{"Item", (UINT)eIconType::Item},
			{"Install", (UINT)eIconType::Install},
			{"Cash", (UINT)eIconType::Cash},
			{"Another", (UINT)eIconType::Another},
			{"SKill", (UINT)eIconType::SKill},

		};
		wstring strPath = PathManager::GetContentPath();
		strPath += L"\\Resources\\GameData\\Item.json";
		
		std::ifstream ifs(strPath.c_str());
		if (!ifs.is_open())
			assert(nullptr);

		json jFile;
		ifs >> jFile;

		for (auto& jData : jFile.at("items")) 
		{
			shared_ptr<tItemInfo> pItem = make_shared<tItemInfo>();
			pItem->iItemID = jData.at("id").get<uint32_t>();
			//pItem.className = jData.at("class").get<std::string>();
			pItem->strItemName = jData.at("name").get<std::string>();
			pItem->eIconType = (eIconType)hashItem[jData.at("iconType").get<std::string>()];
			pItem->iItemLevel = jData.at("level").get<uint32_t>();
			
			m_mapItems.insert(make_pair(pItem->iItemID, pItem));
			m_mapItemName.insert(make_pair(StringToWString(pItem->strItemName), pItem->iItemID));
		}
	}

	void ItemManager::Release()
	{

	}

	weak_ptr<tItemInfo> ItemManager::GetItemInfo(UINT _iItemID)
	{
		auto iter =m_mapItems.find(_iItemID);
		if (iter == m_mapItems.end())
			return {};

		return iter->second;
	}

	weak_ptr<tItemInfo> ItemManager::GetItemInfo(const wstring& _strName)
	{
		auto iter = m_mapItemName.find(_strName);
		if (iter != m_mapItemName.end())
			return GetItemInfo(iter->second);

		return {};
	}

	UINT ItemManager::GetItemID(const wstring& _strName)
	{
		auto iter = m_mapItemName.find(_strName);
		if (iter != m_mapItemName.end())
			return iter->second;

		return 0;
	}

	
}