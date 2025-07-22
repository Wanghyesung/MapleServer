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

PacketHandlerFunc GPacketHandler[UINT16_MAX] = {};

//event버퍼에 넣기
bool Handle_C_ENTER(shared_ptr<Session> _pSession, Protocol::C_ENTER& _pkt)
{
	vector<UINT> vecUserID = GRoom.GetPersons();//이미 서버에 들어온 플레이어

	shared_ptr<ClientSession> pSession = static_pointer_cast<ClientSession>(_pSession);
	const string& _strPersonName = _pkt.name();
	pSession->SetName(_strPersonName);
	UINT iUserID = -1;
	iUserID = GRoom.Enter(_strPersonName, pSession);

	if (iUserID == -1)
		return false;
	pSession->SetPersonID(iUserID);


	UINT64 llEquipIDs = 0;
	int iHairID =0, iEyeID = 0 , iHatID = 0, iTopID = 0 ,
	iBottomID = 0, iShoesID = 0 , iWeaponID = 0;

	DBConnection* pDB = GDBConnectionPool->Pop();
	DBBind<1, 7> dbFind(*pDB, L"SELECT HairID, EyeID, HatID, TopID, BottomID, ShoesID, WeaponID\
							    FROM [dbo].[Equip] WHERE Name = (?)");

	const WCHAR* pName = StringToWString(_strPersonName).c_str();
	dbFind.BindParam(0, pName);
	dbFind.BindCol(0, iHairID); dbFind.BindCol(1, iEyeID);    dbFind.BindCol(2, iHatID);
	dbFind.BindCol(3, iTopID);  dbFind.BindCol(4, iBottomID); dbFind.BindCol(5, iShoesID); dbFind.BindCol(6, iWeaponID);

	if (dbFind.Execute() && dbFind.Fetch())
	{ }
	else
	{
		DBBind<8, 0> dbInster(*pDB, L"INSERT INTO [dbo].[Equip]\
		([Name], [EyeID], [HairID], [HatID], [TopID], [BottomID], [ShoesID], [WeaponID]) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
		iHairID = 0, iEyeID = 0,
		iBottomID = W::ItemManager::GetItemID(L"10_bottom");
		iTopID = W::ItemManager::GetItemID(L"10_top");
		iShoesID = W::ItemManager::GetItemID(L"10_shoes");
		iHatID = W::ItemManager::GetItemID(L"10_hat");
		iWeaponID = W::ItemManager::GetItemID(L"10_weapon");

		dbInster.BindParam(0, pName); dbInster.BindParam(1, iHairID); dbInster.BindParam(2, iEyeID); dbInster.BindParam(3, iBottomID);
		dbInster.BindParam(4, iTopID); dbInster.BindParam(5, iShoesID); dbInster.BindParam(6, iHatID); dbInster.BindParam(7, iWeaponID);

		if (dbInster.Execute() == false)
			assert(nullptr);
	}
	GDBConnectionPool->Push(pDB);

	llEquipIDs = (UINT64)iHairID | ((UINT64)iEyeID << 8) | ((UINT64)iHatID << 16)
		| ((UINT64)iTopID << 24) | ((UINT64)iBottomID << 32) | ((UINT64)iShoesID << 40)
		| ((UINT64)iWeaponID << 48);

	W::EventManager::CreatePlayer(iUserID, llEquipIDs);

	return true;
}

bool Handle_C_EQUIP(shared_ptr<Session> _pSession, Protocol::C_EQUIP& _pkt)
{
	//eventManager로 보내기
	UINT iSceneLayerPlayerIDEquipID = _pkt.scene_layer_playerid_equipid();
	UINT iItemID = _pkt.item_id();
	W::EventManager::ChanagePlayerEquip(iSceneLayerPlayerIDEquipID, iItemID);


	return true;
}

bool Handle_C_INPUT(shared_ptr<Session> _pSession, Protocol::C_INPUT& _pkt)
{
	UINT iPlayerID = _pkt.playerid();

	vector<USHORT> vecKey;
	for (int i = 0; i < _pkt.inpus_size(); ++i)
		vecKey.push_back(_pkt.inpus(i));

	W::EventManager::Update_Input(iPlayerID, vecKey);

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

	W::EventManager::ChanageScene(iPlayerID, iSceneID);

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

	W::EventManager::StartScene(iPlayerID, iSceneID);

	return true;
}


bool Handle_C_SKILL(shared_ptr<Session> _pSession, Protocol::C_Skill& _pkt)
{
	UINT iScenePlayerIDSkillID = _pkt.scene_playerid_skill_id();

	
	UINT iPlayerID = (iScenePlayerIDSkillID >> 8);
	W::Player::ePlayerSkill eSkillID = (W::Player::ePlayerSkill)(iScenePlayerIDSkillID & 0xFF);
	
	W::EventManager::ChangePlayerSkillState(iPlayerID, eSkillID);
	
	return true;
}

bool Handle_C_ITEM(shared_ptr<Session> _pSession, Protocol::C_ITEM& _pkt)
{	
	UINT iScenePlayerIDItemID = _pkt.scene_playerid_itemid();
	W::EventManager::UsingItem(iScenePlayerIDItemID);
	
	return true;
}

bool Handle_C_EXIT(shared_ptr<Session> _pSession, Protocol::C_EXIT& _pkt)
{
	
	return false;
}

