#include "WItemManager.h"
#include "WSceneManger.h"
#include "WPlayer.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "WPathManager.h"


namespace W
{
	using json = nlohmann::json;

	map<UINT, shared_ptr<tItemInfo>> ItemManager::m_mapItems= {};
	map<wstring, UINT> ItemManager::m_mapItemName = {};
	vector<std::function<UINT(UINT)>> ItemManager::m_vecItemEvent = {};

	void ItemManager::Initialize()
	{
		init_function();
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
			UINT iFuncID = jData.value("functionId", -1);
			if (iFuncID != -1)
				pItem->iFunctionID = iFuncID;
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

	int ItemManager::GetItemID(const wstring& _strName)
	{
		auto iter = m_mapItemName.find(_strName);
		if (iter != m_mapItemName.end())
			return iter->second;

		return -1;
	}

	int ItemManager::ExcuteItem(UINT _iItemInfo)
	{
		UCHAR cPlayerID = (_iItemInfo >> 16) & 0xFF;
		UCHAR cItemID = _iItemInfo & 0xFF;
		UINT iSendValue = 0;
		USHORT iFunctionID = 0;
		auto wpItem = GetItemInfo(cItemID);
		if (auto shItem = wpItem.lock())
		{
			iFunctionID = shItem->iFunctionID;
			iSendValue = m_vecItemEvent[iFunctionID](cPlayerID);
		}

		//함수 아이디로 변경
		_iItemInfo &= 0xFFFF0000;
		_iItemInfo |= iFunctionID;

		SendResultItem(_iItemInfo, iSendValue);

		return iSendValue;
	}

	void ItemManager::SendResultItem(UINT _iItemInfo, UINT _iValue)
	{
		UCHAR cSceneID = (_iItemInfo >> 24) & 0xFF;
		
		Protocol::S_ITEM pkt;
		pkt.set_scene_playerid_funcid(_iItemInfo);
		pkt.set_item_value(_iValue);

		shared_ptr<SendBuffer> pBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pBuffer, SceneManger::GetPlayerIDs(cSceneID));
	}

	void ItemManager::init_function()
	{
		m_vecItemEvent.resize(20);

		//ID = Function;
		m_vecItemEvent[1] = ChangeHair;
		m_vecItemEvent[2] = ChangeEye;
	}
	
	UINT ChangeHair(UINT _iPlayerID)
	{
		std::random_device rDiv;
		std::mt19937 en(rDiv());
		std::uniform_int_distribution<UINT> disX(0, 1);
		UINT iHairNum = disX(en);
		iHairNum = 1;

		GameObject* pObj = SceneManger::FindPlayer(_iPlayerID);
		if (pObj)
		{
			Player* pPlayer = static_cast<Player*>(pObj);
			pPlayer->SetHair(iHairNum);
		}
		return iHairNum;
	}

	UINT ChangeEye(UINT _iPlayerID)
	{
		std::random_device rDiv;
		std::mt19937 en(rDiv());
		std::uniform_int_distribution<UINT> disX(0, 2);
		UINT iEyeNum = disX(en);
		
		GameObject* pObj = SceneManger::FindPlayer(_iPlayerID);
		if (pObj)
		{
			Player* pPlayer = static_cast<Player*>(pObj);
			pPlayer->SetEye(iEyeNum);
		}

		return iEyeNum;
	}

}