#pragma once

class Session;
class SendBuffer;
enum eIocpEvent
{
	Send,
	Recv,
	Connect,
	DisConnect,
	Accept
};

class IOCPEvent : public OVERLAPPED
{
public:
	IOCPEvent(eIocpEvent _eType);
	~IOCPEvent();

	void init();

	eIocpEvent GetEvent() { return m_eEventType; }

	void SetOwner(shared_ptr<Session> _pOwner) { m_pOwner = _pOwner; }
	shared_ptr<Session> GetOwner() { return m_pOwner; }
private:
	eIocpEvent m_eEventType;

	shared_ptr<Session> m_pOwner;//session �̺�Ʈ���� �ڱ� �ڽ��� �������� (��ȯ����)

};


class IOCPSendEvent : public IOCPEvent
{
public:
	IOCPSendEvent();
	~IOCPSendEvent();

public:
	vector<shared_ptr<SendBuffer>> m_vecSendBuffer;
};


class IOCPRecvEvent : public IOCPEvent
{
public:
	IOCPRecvEvent();
	~IOCPRecvEvent();
};

class IOCPConnectEvent : public IOCPEvent
{
public:
	IOCPConnectEvent();
	~IOCPConnectEvent();
};

class ICOPDisConnectEvent : public IOCPEvent
{
public:
	ICOPDisConnectEvent();
	~ICOPDisConnectEvent();
};

class IOCPAcceptEvent : public IOCPEvent
{
public:
	IOCPAcceptEvent();
	~IOCPAcceptEvent();

public:
	//���� ����� Ŭ�����
	shared_ptr<Session> m_pSession = nullptr;
};
