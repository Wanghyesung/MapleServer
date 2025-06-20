#pragma once

#include "PacketSession.h"
#include "Enter.pb.h"
#include "Exit.pb.h"
#include "Equip.pb.h"
#include "Input.pb.h"
#include "Map.pb.h"
#include "Transform.pb.h"
#include "GameObject.pb.h"
#include "ObjectState.pb.h"
#include "Skill.pb.h"

using PacketHandlerFunc = std::function<bool(shared_ptr<PacketSession>&, BYTE*, INT)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

extern unordered_map<string, wstring> GHashName;
//기본 : ID, LAYER, SCENE

enum PACKET_TYPE
{
	S_ENTER = 1000,
	C_ENTER = 1001, //클라가 서버에게
	S_NEW_ENTER = 1002,

	S_COLLISION = 1003,

	S_EQUIP = 1004,
	C_EQUIP = 1005, //클라가 서버에게

	C_INPUT = 1006,

	S_MAP = 1007,
	C_MAP = 1008,

	S_START_MAP = 1009,
	C_START_MAP = 1010,

	S_CREATE = 1011,
	C_CREATE = 1012,
	S_DELETE = 1013,

		
	//물체 위치
	S_STATE = 1014,

	S_TRANSFORM = 1015,

	S_SKILL = 1016,
	C_SKILL = 1017,

	S_EXIT = 1018,
	C_EXIT = 1019,
	S_NEW_EXIT = 1020,
};


template <typename T>
shared_ptr<SendBuffer> _MakeSendBuffer(T& _pkt, UINT _ID);

//클라에서 온 패킷 처리
bool Handle_C_ENTER(shared_ptr<Session> _pSession, Protocol::C_ENTER& _pkt);
bool Handle_C_EQUIP(shared_ptr<Session> _pSession, Protocol::C_EQUIP& _pkt);
bool Handle_C_INPUT(shared_ptr<Session> _pSession, Protocol::C_INPUT& _pkt);
bool Handle_C_CREATE(shared_ptr<Session> _pSession, Protocol::C_CREATE& _pkt);
bool Handle_C_MAP(shared_ptr<Session> _pSession, Protocol::C_MAP& _pkt);
bool Handle_C_EXIT(shared_ptr<Session> _pSession, Protocol::C_EXIT& _pkt);
bool Handle_C_START_MAP(shared_ptr<Session> _pSession, Protocol::C_START_MAP& _pkt);
bool Handle_C_SKILL(shared_ptr<Session> _pSession, Protocol::C_Skill& _pkt);


class ClientPacketHandler
{
public:
	static bool HandlePacket(shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
	{
		PacketHeader* pHeader = reinterpret_cast<PacketHeader*>(_pBuffer);
		return GPacketHandler[pHeader->id](_pSession, _pBuffer, _iLen);
	}

	static void Initialize()
	{
		GPacketHandler[C_ENTER] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::C_ENTER>(Handle_C_ENTER, _pSession, _pBuffer, _iLen); };
		GPacketHandler[C_EQUIP] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::C_EQUIP>(Handle_C_EQUIP, _pSession, _pBuffer, _iLen); };
		GPacketHandler[C_INPUT] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::C_INPUT>(Handle_C_INPUT, _pSession, _pBuffer, _iLen); };
		GPacketHandler[C_CREATE] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::C_CREATE>(Handle_C_CREATE, _pSession, _pBuffer, _iLen); };
		GPacketHandler[C_MAP] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::C_MAP>(Handle_C_MAP, _pSession, _pBuffer, _iLen); };
		GPacketHandler[C_SKILL] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::C_Skill>(Handle_C_SKILL, _pSession, _pBuffer, _iLen); };
		GPacketHandler[C_START_MAP] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::C_START_MAP>(Handle_C_START_MAP, _pSession, _pBuffer, _iLen); };
		GPacketHandler[C_EXIT] = [](shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
			{return  HandlePacket<Protocol::C_EXIT>(Handle_C_EXIT, _pSession, _pBuffer, _iLen); };
	}

	template <typename T, typename Func>
	static bool HandlePacket(Func _pFunc, shared_ptr<PacketSession>& _pSession, BYTE* _pBuffer, INT _iLen)
	{
		T pkt;
		if (pkt.ParseFromArray(_pBuffer + sizeof(PacketHeader), _iLen - sizeof(PacketHeader)) == false)
			return false;

		return _pFunc(_pSession, pkt);
	}
	
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_ENTER _pkt) { return _MakeSendBuffer(_pkt, S_ENTER); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_NEW_ENTER _pkt) { return _MakeSendBuffer(_pkt, S_NEW_ENTER); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_NEW_EXIT _pkt) { return _MakeSendBuffer(_pkt, S_NEW_EXIT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_EQUIP _pkt) { return _MakeSendBuffer(_pkt, S_EQUIP); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_CREATE _pkt) { return _MakeSendBuffer(_pkt, S_CREATE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_DELETE _pkt) { return _MakeSendBuffer(_pkt, S_DELETE); }
	
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MAP _pkt) { return _MakeSendBuffer(_pkt, S_MAP); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_STATE _pkt) { return _MakeSendBuffer(_pkt, S_STATE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_TRANSFORM _pkt) { return _MakeSendBuffer(_pkt, S_TRANSFORM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_Skill _pkt) { return _MakeSendBuffer(_pkt, S_SKILL); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_START_MAP _pkt) { return _MakeSendBuffer(_pkt, C_START_MAP); }

	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_EXIT _pkt) { return _MakeSendBuffer(_pkt, S_EXIT); }

};



template<typename T>
inline shared_ptr<SendBuffer> _MakeSendBuffer(T& _pkt, UINT _ID)
{
	const UINT16 iDataSize = static_cast<UINT16>(_pkt.ByteSizeLong());
	const UINT16 iPacketSize = iDataSize + sizeof(PacketHeader);
	shared_ptr<SendBuffer> pSendBuffer = SendBufferMgr->Open(iPacketSize);

	PacketHeader* pHeader = reinterpret_cast<PacketHeader*>(pSendBuffer->GetData());
	pHeader->id = _ID;
	pHeader->size = iPacketSize;
	_pkt.SerializeToArray(&pHeader[1], iDataSize);

	pSendBuffer->Close(iPacketSize);
	return pSendBuffer;
}
