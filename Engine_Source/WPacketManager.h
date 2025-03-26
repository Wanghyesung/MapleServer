#pragma once
#include "Engine.h"


namespace W
{
	class Packet;

	class PacketManager
	{
	public:
		//Engine���� ���������� ���� Lock X
		static void AddPacket(Packet* _pPacket);
		static void Update();
	private:
		static std::queue<Packet*> m_queuePacket;

		//�޴����� �̺�Ʈ �Ŵ����� �ٷ� �ֱ�
		//static std::queue<Packet*> m_queueRecvPacket;
	};
}
