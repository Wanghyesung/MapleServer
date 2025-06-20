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
// 반드시 Windows.h 전에 WinSock2 관련 포함
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

#include <vector> // 배열
#include <list> //링크드리스트 
#include <map> // 2진트리
#include <unordered_map>
#include <queue>
#include <bitset> // 비트 배열 편하게 사용해주는 라이브러리
#include <set> // 해시 테이블
#include <functional> // 함수 포인터
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <cmath> //수학
#include <algorithm> //정렬 알고리즘
#include <limits> //부동 소수점 표현 및 반올림과 관련된 클래스 템플릿 numeric_limits 및 두 개의 열거형을 정의합니다.
#include <memory> //메모리 관련된 라이브러리


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