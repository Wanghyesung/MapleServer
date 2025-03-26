#pragma once

#include "PacketSession.h"

using PacketHandlerFunc = std::function<bool(shared_ptr<PacketSession>&, BYTE*, INT)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];
//�⺻ : ID, LAYER, SCENE

enum PACKET_TYPE
{
	S_ENTER = 1000,
	C_ENTER = 1001, //Ŭ�� ��������
	S_NEW_ENTER = 1002,

	//��ü ��ġ
	S_TRANSFORM = 1003,

	//�� ��ü
	/*
	{
  "1001": {
    "name": "Goblin",
    "model": "Models/Goblin.fbx",
    "anim_idle": "Anims/goblin_idle.anim",
    "anim_attack": "Anims/goblin_attack.anim"
  }
	�����ɶ��� ������ Ŭ�󿡼��� ���ҽ� �ε�
	�� �Ŀ��� �ִϸ��̼� �ε����� ������ STATE : STATE : string, ANIM_IDX : int
	*/
	S_NEW_OBJECT = 1006,

	//�� �̵�
	S_CHANAGE_MAP = 1007,

	//��ü ����
	S_PLAYER_STATE = 1008,

	//Ŭ�� �������� ���� Ű�� ����
	C_PLAYER_INPUT = 1009,

	//�浹
	S_COLLISION = 1010,

	//Ŭ�� �κ����� �� ����
	S_EQUIP = 1011,
	C_EQUIP = 1012,
	S_NEW_EQUIP = 1012,


	S_EXIT = 1013,
	C_EXIT = 1014,
	S_NEW_EXIT = 1015,
};


template <typename T>
shared_ptr<SendBuffer> _MakeSendBuffer(T& _pkt, UINT _ID);

class ServerPacketHandler
{
	static bool HandlePacket(shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
	{
		PacketHeader* pHeader = reinterpret_cast<PacketHeader*>(_pBuffer);
		return GPacketHandler[pHeader->id](_pSession, _pBuffer, _iLen);
	}

	static void Initialize()
	{
		GPacketHandler[S_ENTER] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return 1; };
	}

	template <typename T, typename Func>
	static bool HandlePacket(Func _pFunc, shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
	{
		T pkt;
		if (pkt.ParseFromArray(_pBuffer + sizeof(PacketHeader), _iLen - sizeof(PacketHeader)) == false)
			return false;

		return _pFunc(_pSession, pkt);
	}
};


template<typename T>
inline shared_ptr<SendBuffer> _MakeSendBuffer(T& _pkt, UINT _ID)
{
	return shared_ptr<SendBuffer>();
}
