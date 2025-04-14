#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"
#include "Room.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX] = {};
//event버퍼에 넣기
bool Handle_C_ENTER(shared_ptr<Session> _pSession, Protocol::C_ENTER& _pkt)
{
	shared_ptr<ClientSession> pSession = static_pointer_cast<ClientSession>(_pSession);
	const string& _strPersonName = _pkt.name();
	pSession->SetName(_strPersonName);
	//성공했다면
	if (GRoom.Enter(_strPersonName,pSession) == false)
		return false;

	//다른 클라들에게 전송
	Protocol::S_NEW_ENTER other_pkt;
	
	shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(other_pkt);
	GRoom.Broadcast(pSendBuffer);
	
	vector<UINT> vecUserID = GRoom.GetPersons();
	Protocol::S_ENTER pkt;
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

	return false;
}

bool Handle_C_CREATE(shared_ptr<Session> _pSession, Protocol::C_CREATE& _pkt)
{

	return false;
}

bool Handle_C_EXIT(shared_ptr<Session> _pSession, Protocol::C_EXIT& _pkt)
{

	return false;
}
