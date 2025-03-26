#include "pch.h"
#include "Listener.h"
#include "IOCP.h"
#include "IOCPEvent.h"
#include "SockHelper.h"
#include "Service.h"
#include "NetAddress.h"

Listener::Listener()
{

}

Listener::~Listener()
{
	SockHelper::Close(m_socket);

	for (IOCPAcceptEvent* pEvent : m_vecEvent)
	{
		delete pEvent;
	}
}

void Listener::Clear()
{
	for (IOCPAcceptEvent* pEvent : m_vecEvent)
	{
		pEvent->SetOwner(nullptr);
	}
}

void Listener::Start()
{
	m_socket = SockHelper::Create_Socket();
	
	
	GetService()->GetIOCP()->RegisterEvent(shared_from_this());
	
	if (SockHelper::SetReuseAddress(m_socket, true) == false)
		assert(nullptr);

	if (SockHelper::SetLinger(m_socket, 0, 0) == false)
		assert(nullptr);
	
	if (SockHelper::Bind(m_socket, GetService()->GetAddress()) == false)
		assert(nullptr);

	if (SockHelper::Listen(m_socket, SOMAXCONN) == false)
		assert(nullptr);
	

	UINT AcceptCount = GetService()->GetMaxSessionCount();

	for (UINT i = 0; i < AcceptCount; ++i)
	{
		IOCPAcceptEvent* pAcceptEvent = new IOCPAcceptEvent();
		pAcceptEvent->SetOwner(shared_from_this()); //�� listensocket ���

		m_vecEvent.push_back(pAcceptEvent);
		RegisterAccept(pAcceptEvent);
	}
}

void Listener::DisPatch(IOCPEvent* _pEvent, int _iNumOfBytes)
{
	//��� event�� ����ϰ� ������� ������ �߻� -> shared�� �ٲٱ�
	IOCPAcceptEvent* pAccept = static_cast<IOCPAcceptEvent*>(_pEvent);
	pAccept->GetOwner()->ProcessAccept(pAccept);
	
}

void Listener::RegisterAccept(IOCPAcceptEvent* _pAcceptEvent)
{
	//Ŭ�� ����
	shared_ptr<Session> pSession = GetService()->CreateSession(); //���⼭ Ŭ������ ��������
	
	_pAcceptEvent->init();
	_pAcceptEvent->m_pSession = pSession; //�� Ŭ����� ��� (�������� Ŭ��� �����͸� �ְ�ޱ� ����)


	DWORD bytesRecved = 0;
	
	if (false == SockHelper::AcceptEx(m_socket, pSession->GetSocket(), pSession->m_recvBuffer.GetWritePos(), 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytesRecved, reinterpret_cast<LPOVERLAPPED>(_pAcceptEvent)))
	{
		const int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			//���� �����ϸ� �ٽ� accept
			RegisterAccept(_pAcceptEvent);
		}
	}
}

void Listener::ProcessAccept(IOCPAcceptEvent* _pAcceptEvent)
{
	shared_ptr<Session> session = _pAcceptEvent->m_pSession; //�Ʊ� ������ Ŭ�����

	//������ (�������� Ŭ��� �����ϱ����� ��Ĺ)
	if (SockHelper::SetUpdateAcceptSocket(session->GetSocket(), m_socket) == false)
		assert(nullptr);

	SOCKADDR_IN clientAddr;
	int iAddrSize = sizeof(clientAddr);
	if (getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&clientAddr), &iAddrSize) == SOCKET_ERROR)
	{
		RegisterAccept(_pAcceptEvent);
	}

	session->SetAddress(NetAddress(clientAddr));

	//connected
	session->ProcessConnect();
	
	RegisterAccept(_pAcceptEvent);

}
