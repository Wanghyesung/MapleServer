#pragma once
#include "PacketSession.h"

class ClientSession : public PacketSession
{
public:
    ClientSession();
    virtual ~ClientSession();

    virtual void OnConnected();
    virtual int OnRecvPacket(BYTE* buffer, int len);
    virtual void OnSend(int len);
    virtual void OnDisConnected();

private:


};

