#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"

#include "Room.h"
#include "WEventManager.h"
#include "NetFunc.h"
#include "..\Engine_Source\WSceneManger.h"
#include "..\Engine_Source\WLayer.h"
#include "..\Engine_Source\WAnimator.h"
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

	W::EventManager::CreatePlayer(iUserID);

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

	//eventmgr에 보내기
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

bool Handle_C_EXIT(shared_ptr<Session> _pSession, Protocol::C_EXIT& _pkt)
{
	
	return false;
}

