#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"

#include "Room.h"
#include "WEventManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX] = {};
//event���ۿ� �ֱ�
bool Handle_C_ENTER(shared_ptr<Session> _pSession, Protocol::C_ENTER& _pkt)
{
	shared_ptr<ClientSession> pSession = static_pointer_cast<ClientSession>(_pSession);
	const string& _strPersonName = _pkt.name();
	pSession->SetName(_strPersonName);
	UINT iUserID = -1;

	vector<UINT> vecUserID = GRoom.GetPersons();//�̹� ������ ���� �÷��̾�

	//�����ߴٸ�
	if (iUserID = GRoom.Enter(_strPersonName,pSession) == -1)
		return false;

	//�ٸ� Ŭ��鿡�� ����
	Protocol::S_NEW_ENTER other_pkt;
	other_pkt.set_playerid(iUserID);
	shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(other_pkt);
	GRoom.Broadcast(pSendBuffer);
	
	Protocol::S_ENTER pkt;
	pkt.set_playerid(iUserID);
	pkt.set_success(true);
	for (int i = 0; i < vecUserID.size(); ++i)
	{
		pkt.add_users_ids(vecUserID[i]);
	}
	pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	_pSession->Send(pSendBuffer);

	GRoom.Enter(_strPersonName, _pSession);
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
	{
		vecKey.push_back(_pkt.inpus(i));
	}

	//eventmgr�� ������
	W::EventManager::Update_Input(iPlayerID, vecKey);

	return true;
}

bool Handle_C_CREATE(shared_ptr<Session> _pSession, Protocol::C_CREATE& _pkt)
{

	return false;
}

bool Handle_C_EXIT(shared_ptr<Session> _pSession, Protocol::C_EXIT& _pkt)
{

	return false;
}
