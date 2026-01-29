#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"

#include "Room.h"
#include "WEventManager.h"
#include "NetFunc.h"
#include "BindDB.h"
#include "..\Engine_Source\WSceneManger.h"
#include "..\Engine_Source\WLayer.h"
#include "..\Engine_Source\WAnimator.h"
#include "..\Engine\WItemManager.h"

using namespace W;

PacketHandlerFunc GPacketHandler[UINT16_MAX] = {};
wstring GarrEquipName[(UINT)W::eEquipType::End] = { L"", L"PandantID,", L"HatID", L"TopID", L"BottomID", L"ShoesID", L"WeaponID"};
wstring GarrAppearanceName[(UINT)W::eAppearance::End] = {L"",L"HairID" ,L"EyeID"};


bool Handle_C_ENTER(shared_ptr<Session> _pSession, Protocol::C_ENTER& _pkt)
{
	shared_ptr<ClientSession> pSession = static_pointer_cast<ClientSession>(_pSession);
	const string& strPersonName = _pkt.name();
	pSession->SetName(strPersonName);
	int iUserID = -1;
	iUserID = GRoom.Enter(strPersonName, pSession);

	if (iUserID == -1)
		return false;
	pSession->SetPersonID(iUserID);

	UINT64 llEquipIDs = 0;
	int iHairID =0, iEyeID = 0 , iHatID = 0, iTopID = 0 ,
		iBottomID = 0, iShoesID = 0 , iWeaponID = 0;

	DBConnection* pDB = GDBConnectionPool->Pop();
	DBBind<1, 7> dbFind(*pDB, L"SELECT HairID, EyeID, HatID, TopID, BottomID, ShoesID, WeaponID\
							    FROM [dbo].[Equip] WHERE Name = (?)");

	const WCHAR* pName = StringToWString(strPersonName).c_str();
	dbFind.BindParam(0, pName);
	dbFind.BindCol(0, iHairID); dbFind.BindCol(1, iEyeID);    dbFind.BindCol(2, iHatID);
	dbFind.BindCol(3, iTopID);  dbFind.BindCol(4, iBottomID); dbFind.BindCol(5, iShoesID); dbFind.BindCol(6, iWeaponID);

	if (dbFind.Execute() && dbFind.Fetch())
	{ }
	else
	{
		DBBind<8, 0> dbInster(*pDB, L"INSERT INTO [dbo].[Equip]\
		([Name], [HairID], [EyeID], [HatID], [TopID], [BottomID], [ShoesID], [WeaponID]) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
		iHairID = 0, iEyeID = 0,
		iBottomID = ItemManager::GetItemID(L"10_bottom"); iTopID = ItemManager::GetItemID(L"10_top"); iShoesID = ItemManager::GetItemID(L"10_shoes");
		iHatID = ItemManager::GetItemID(L"10_hat"); iWeaponID = ItemManager::GetItemID(L"10_weapon");

		dbInster.BindParam(0, pName); dbInster.BindParam(1, iHairID); dbInster.BindParam(2, iEyeID); dbInster.BindParam(3, iHatID);
		dbInster.BindParam(4, iTopID); dbInster.BindParam(5, iBottomID); dbInster.BindParam(6, iShoesID); dbInster.BindParam(7, iWeaponID);

		if (dbInster.Execute() == false)
			assert(nullptr);
	}
	GDBConnectionPool->Push(pDB);

	llEquipIDs = (UINT64)iHairID | ((UINT64)iEyeID << 8) | ((UINT64)iHatID << 16)
		| ((UINT64)iTopID << 24) | ((UINT64)iBottomID << 32) | ((UINT64)iShoesID << 40)
		| ((UINT64)iWeaponID << 48);

	EventManager::CreatePlayer(iUserID, llEquipIDs);

	return true;
}

bool Handle_C_EQUIP(shared_ptr<Session> _pSession, Protocol::C_EQUIP& _pkt)
{
	shared_ptr<ClientSession> pSession = static_pointer_cast<ClientSession>(_pSession);
	const wstring& strPlayerName =  StringToWString(pSession->GetName());

	UINT iSceneLayerPlayerIDEquipID = _pkt.scene_layer_playerid_equipid();
	int iItemID = _pkt.item_id();
	
	//DB¿¡ ³Ö±â
	UCHAR cEquipID = iSceneLayerPlayerIDEquipID & 0xFF;
	if (cEquipID == 0 || cEquipID >= (UINT)eEquipType::End)
		return false;

	const wstring& strEquipName = GarrEquipName[cEquipID];
	DBConnection* pDB = GDBConnectionPool->Pop();

	wstring query = L"UPDATE [dbo].[Equip] SET [" + strEquipName + L"] = ? WHERE[Name] = ?";
	DBBind<2, 0> pDBUpdate(*pDB, query.c_str());
	pDBUpdate.BindParam(0, iItemID);
	pDBUpdate.BindParam(1, strPlayerName.c_str());
	if (pDBUpdate.Execute() == false)
		assert(nullptr);

	GDBConnectionPool->Push(pDB);
	
	EventManager::ChanagePlayerEquip(iSceneLayerPlayerIDEquipID, iItemID);

	return true;
}

bool Handle_C_INPUT(shared_ptr<Session> _pSession, Protocol::C_INPUT& _pkt)
{
	UINT iPlayerID = _pkt.playerid();

	vector<USHORT> vecKey;
	for (int i = 0; i < _pkt.inpus_size(); ++i)
		vecKey.push_back(_pkt.inpus(i));

	EventManager::Update_Input(iPlayerID, vecKey);

	return true;
}

bool Handle_C_CREATE(shared_ptr<Session> _pSession, Protocol::C_CREATE& _pkt)
{

	return false;
}

bool Handle_C_MAP(shared_ptr<Session> _pSession, Protocol::C_MAP& _pkt)
{
	UINT iScene_playerID = _pkt.scene_player_id();	
	USHORT iSceneID = (iScene_playerID >> 16) & 0xFFFF;
	USHORT iPlayerID = iScene_playerID & 0xFFFF;
	GRoom.UnLockSendBask(iPlayerID);

	EventManager::ChanageScene(iPlayerID, iSceneID);

	return true;
}

bool Handle_C_MAP_LOADING(shared_ptr<Session> _pSession, Protocol::C_MAP_LOADING& _pkt)
{
	UINT iPlayerID = _pkt.player_id();
	GRoom.LockSendBask(iPlayerID);

	return true;
}

bool Handle_C_START_MAP(shared_ptr<Session> _pSession, Protocol::C_START_MAP& _pkt)
{
	UINT iScene_playerID = _pkt.scene_player_id();
	USHORT iSceneID = (iScene_playerID >> 16) & 0xFFFF;
	USHORT iPlayerID = iScene_playerID & 0xFFFF;

	EventManager::StartScene(iPlayerID, iSceneID);

	return true;
}


bool Handle_C_SKILL(shared_ptr<Session> _pSession, Protocol::C_Skill& _pkt)
{
	UINT iScenePlayerIDSkillID = _pkt.scene_playerid_skill_id();

	
	UINT iPlayerID = (iScenePlayerIDSkillID >> 8);
	Player::ePlayerSkill eSkillID = (Player::ePlayerSkill)(iScenePlayerIDSkillID & 0xFF);
	
	EventManager::ChangePlayerSkillState(iPlayerID, eSkillID);
	
	return true;
}

bool Handle_C_ITEM(shared_ptr<Session> _pSession, Protocol::C_ITEM& _pkt)
{	
	shared_ptr<ClientSession> pSession = static_pointer_cast<ClientSession>(_pSession);
	const wstring& strPlayerName = StringToWString(pSession->GetName());

	UINT iScenePlayerIDItemID = _pkt.scene_playerid_itemid();	
	UINT iAppearID = (UCHAR)(iScenePlayerIDItemID >> 8);
	int iItemID = (UCHAR)iScenePlayerIDItemID;
	if (iAppearID == 0 || iAppearID >= (UINT)eAppearance::End)
		return false;

	const wstring& strAppearName = GarrAppearanceName[iAppearID];
	DBConnection* pDB = GDBConnectionPool->Pop();
	int iRetValue = ItemManager::ExcuteItem(iScenePlayerIDItemID);
	
	wstring query = L"UPDATE [dbo].[Equip] SET [" + strAppearName + L"] = ? WHERE[Name] = ?";
	DBBind<2, 0> pDBUpdate(*pDB, query.c_str());
	pDBUpdate.BindParam(0, iRetValue);
	pDBUpdate.BindParam(1, strPlayerName.c_str());
	if (pDBUpdate.Execute() == false)
		assert(nullptr);

	GDBConnectionPool->Push(pDB);

	return true;
}

