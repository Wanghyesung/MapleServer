#pragma once
class Session;

class IOCP
{
public:
	IOCP();
	~IOCP();
	
public:
	void RegisterEvent(shared_ptr<Session> _pSession);
	void RegisterDummyPacket();
	void Execute();

private:
	HANDLE m_IOCPHandle;
};

