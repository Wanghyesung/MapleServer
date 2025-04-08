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

    const string& GetName() { return m_strName; }
    void SetName(const string& _strName) { m_strName = _strName; }
public:
    const UINT GetPersonID() { return m_iUserID; }
    void SetPersonID(UINT _iUserID) { m_iUserID = _iUserID; }

private:
    UINT m_iUserID;
    string m_strName;
};

