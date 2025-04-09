#include "pch.h"
#include "ClientSession.h"
#include "Room.h"
#include "ClientPacketHandler.h"

ClientSession::ClientSession()
{

}

ClientSession::~ClientSession()
{

}

void ClientSession::OnConnected()
{
    int a = 10;
}

int ClientSession::OnRecvPacket(BYTE* buffer, int len)
{
    shared_ptr<PacketSession> pSession = GetPacketSessionRef();
    ClientPacketHandler::HandlePacket(pSession, buffer, len);

    return len;
    
}

void ClientSession::OnSend(int len)
{

}

void ClientSession::OnDisConnected()
{
    //GRoom.Exit(m_iName)
}
