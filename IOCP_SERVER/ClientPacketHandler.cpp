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

//event���ۿ� �ֱ�
bool Handle_C_ENTER(shared_ptr<Session> _pSession, Protocol::C_ENTER& _pkt)
{
	vector<UINT> vecUserID = GRoom.GetPersons();//�̹� ������ ���� �÷��̾�

	shared_ptr<ClientSession> pSession = static_pointer_cast<ClientSession>(_pSession);
	const string& _strPersonName = _pkt.name();
	pSession->SetName(_strPersonName);
	UINT iUserID = -1;
	iUserID = GRoom.Enter(_strPersonName, pSession);

	if (iUserID == -1)
		return false;

	//�ٸ� Ŭ��鿡�� ����
	Protocol::S_NEW_ENTER other_pkt;
	other_pkt.set_playerid(iUserID);
	shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(other_pkt);
	GRoom.BroadcastExcept(pSendBuffer,_pSession);
	
	//����� Ŭ�󿡰� ����
	Protocol::S_ENTER pkt;
	pkt.set_player_id(iUserID);
	
	pkt.set_success(true);
	
	for (int i = 0; i < vecUserID.size(); ++i)
	{
		pkt.add_player_ids(vecUserID[i]);
	}
	pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	_pSession->Send(pSendBuffer);

	GRoom.Enter(_strPersonName, _pSession);

	W::EventManager::CreatePlayer(iUserID);
	return true;

}

bool Handle_C_EQUIP(shared_ptr<Session> _pSession, Protocol::C_EQUIP& _pkt)
{

	return false;
}

bool Handle_C_INPUT(shared_ptr<Session> _pSession, Protocol::C_INPUT& _pkt)
{
	UINT iPlayerID = _pkt.playerid();

	vector<USHORT> vecKey;
	for (int i = 0; i < _pkt.inpus_size(); ++i)
		vecKey.push_back(_pkt.inpus(i));

	//eventmgr�� ������
	W::EventManager::Update_Input(iPlayerID, vecKey);

	return true;
}

bool Handle_C_CREATE(shared_ptr<Session> _pSession, Protocol::C_CREATE& _pkt)
{

	return false;
}

bool Handle_C_MAP(shared_ptr<Session> _pSession, Protocol::C_MAP& _pkt)
{
	
	UINT iPlayerID = _pkt.player_id();	

	const wstring& strScene = StringToWString(_pkt.scene());
	
	W::EventManager::ChanageScene(iPlayerID, strScene);
	
	return true;
}

bool Handle_C_START_MAP(shared_ptr<Session> _pSession, Protocol::C_START_MAP& _pkt)
{
	UINT iPlayerID = _pkt.player_id();

	const wstring& strScene = StringToWString(_pkt.scene());
	
	W::EventManager::StartScene(iPlayerID, strScene);

	return true;
}


bool Handle_C_SKILL(shared_ptr<Session> _pSession, Protocol::C_Skill& _pkt)
{
	W::Player::ePlayerSkill eSkillID = (W::Player::ePlayerSkill)_pkt.skill_id();
	UINT iPlayerID = _pkt.player_id();

	const wstring& strScene = StringToWString(_pkt.scene());

	W::EventManager::ChangePlayerSkillState(iPlayerID, eSkillID);
	
	return true;
}

bool Handle_C_EXIT(shared_ptr<Session> _pSession, Protocol::C_EXIT& _pkt)
{

	return false;
}

