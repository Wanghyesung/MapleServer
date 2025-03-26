#pragma once

#include "NetAddress.h"
#include "IOCPEvent.h"
#include "RecvBuffer.h"
#include "SendBuffer.h"

class IOCPEvent;
class Service;
class RecvBuffer;

enum
{
	BUFFER_SIZE = 0x10000
};

class Session : public enable_shared_from_this<Session>
{
	friend class Listener;

public:
	Session();
	virtual ~Session();

	virtual void DisPatch(IOCPEvent* _pEvent, int _iNumOfBytes);

	void SetService(shared_ptr<Service> _pService) { m_weakService = _pService; }
	void SetAddress(NetAddress _Address) { m_Address = _Address; }

	HANDLE GetHandle() { return reinterpret_cast<HANDLE>(m_socket);}
	SOCKET GetSocket() { return m_socket; }
	shared_ptr<Service> GetService() { return m_weakService.lock(); }

	virtual void Clear();
public:
	virtual void RegisterAccept(IOCPAcceptEvent* _pIocpEvent) {};
	virtual void ProcessAccept(IOCPAcceptEvent* _pIocpEvent) {};

	virtual void RegisterConnect();
	virtual void ProcessConnect();

	virtual void RegisterDisConnect();
	virtual void ProcessDisConnect();

	virtual void RegisterSend();
	virtual void ProcessSend(int _iNumOfBytes);

	virtual void RegisterRecv();
	virtual void ProcessRecv(int _iNumOfBytes);

	void HandleError(int iErrorCode);
public:
	void DisConnect(const WCHAR* _strCause);
	void Connect();
	void Send(shared_ptr<SendBuffer> _pBuffer);
	//void Recv();
protected:
	//���� ������ �Ǿ��� �� ������ �Լ� 
	//(Ư�� ������ �Ǿ��� �� ������ �Լ���)
	virtual void OnConnected() {};
	virtual int OnRecv(BYTE* buffer, int len) { return len; }
	virtual void OnSend(int len) {}
	virtual void OnDisConnected() {}


private:
	SOCKET m_socket;
	weak_ptr<Service> m_weakService;//��ȯ ����
	NetAddress m_Address;
	atomic<bool> m_bConnected;

	IOCPConnectEvent m_ConnectEvent;
	ICOPDisConnectEvent m_DisConnectEvent;
	IOCPSendEvent m_sendEvent;
	IOCPRecvEvent m_recvEvent;


private:
	RWLock m_lock;


	//IOCP�� ���ÿ� �� �� �̻��� Recv �۾��� ���� ���Ͽ��� ó������ �ʽ��ϴ�.
	RecvBuffer m_recvBuffer;

	//���� ���� ��ü(Session)�� ���� ���� �����尡 ���ÿ� ������ ���ɼ��� �ֽ��ϴ�
	queue<shared_ptr<SendBuffer>> m_qSendBuffer;
	atomic<bool> m_atRegisterSend;

	/*
	16-3-2 ������ �����ߴµ� 3�� 16����Ʈ �߰��� ����� ���۵� ���� �����ϴ�. �ٸ� 16+3 �̷��� 19����Ʈ�� �� ���� �� ���� �ְ���.
	*/

};

