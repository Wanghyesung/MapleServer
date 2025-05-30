#pragma once

#ifdef _DEBUG
#pragma comment(lib, "IOCP\\Debug\\IOCP.lib")
#pragma comment(lib, "ProtoBuf\\Debug\\libprotobufd.lib")
#pragma comment (lib,"IOCP_SERVER\\Debug\\IOCP_SERVER.lib")

#else
//#pragma comment (lib, "..\\x64\\Release\\Engine.lib")
#pragma comment(lib, "IOCP\\Release\\IOCP.lib")
#pragma comment(lib, "ProtoBuf\\Release\\libprotobufd.lib")
#pragma comment (lib,".IOCP_SERVER\\Release\\IOCP_SERVER.lib")

#endif

/*/////////////////////////
		  IOCP
*/////////////////////////
// �ݵ�� Windows.h ���� WinSock2 ���� ����
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#pragma comment(lib, "ws2_32.lib")

#include <mutex>
#include <atomic>

#include "RWLock.h"

#include "container.h"#
#include "Global.h"
#include "SendBufferChunk.h"

#include <string>
#include <Windows.h>
#include <assert.h>

#include <vector> // �迭
#include <list> //��ũ�帮��Ʈ 
#include <map> // 2��Ʈ��
#include <unordered_map>
#include <queue>
#include <bitset> // ��Ʈ �迭 ���ϰ� ������ִ� ���̺귯��
#include <set> // �ؽ� ���̺�
#include <functional> // �Լ� ������
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <cmath> //����
#include <algorithm> //���� �˰���
#include <limits> //�ε� �Ҽ��� ǥ�� �� �ݿø��� ���õ� Ŭ���� ���ø� numeric_limits �� �� ���� �������� �����մϴ�.
#include <memory> //�޸� ���õ� ���̺귯��


// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;

#include <random>

#include "Common.h"
#include "Enums.h"
#include "WMath.h"


#include "..\IOCP_SERVER\NetFunc.h"
#include "..\IOCP_SERVER\ClientPacketHandler.h"
#include "..\IOCP_SERVER\Room.h"

extern Room GRoom;