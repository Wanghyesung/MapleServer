﻿//정적라이브러리 추가 1. 소스코드  2. 비주얼스튜디오 옵션
//정적라이브러리에 들어가기 위한 헤더파일 include 

//참조 -> 엔진소수 추가


//정적라이브러리의 commoninclude헤더파일 include해야 알수있음
//#include "..\Client_Source\CommonInclude.h"

////하나하나 전부 해줄 수 없기 때문에
//#include "CommonInclude.h"
//
////디버그일떄 Debug파일에 정적라이브러리를 참조하고 Release일때는 Release파일에 정적라이브러리 참조
#ifdef _DEBUG
#pragma comment (lib,"..\\x64\\Debug\\Engine.lib")
#pragma comment(lib, "IOCP\\Debug\\IOCP.lib")
#pragma comment(lib, "ProtoBuf\\Debug\\libprotobufd.lib")

#else
#pragma comment (lib, "..\\x64\\Release\\Engine.lib")
#pragma comment(lib, "IOCP\\Release\\IOCP.lib")
#pragma comment(lib, "ProtoBuf\\Release\\libprotobufd.lib")

#endif


#include "framework.h"
#include "Client.h"
#include "..\Engine_Source\WApplication.h"
#include "..\Engine_Source\WResources.h"

#include "..\Engine_Source/WFmod.h"
#include "..\Engine_Source\WFontWrapper.h"
#include "..\Engine\LoadScene.h"
#include "..\Engine_Source\WThreadPool.h"

#ifdef _DEBUG
#pragma comment(lib, "IOCP\\Debug\\IOCP.lib")
#pragma comment(lib, "IOCP_SERVER\\Debug\\IOCP_SERVER.lib")
#else
#pragma comment(lib, "IOCP\\Release\\IOCP.lib")
#pragma comment(lib, "IOCP_SERVER\\Release\\IOCP_SERVER.lib")
#endif

#include "..\IOCP\pch.h"

#include "..\IOCP\SockHelper.h"
#include "..\IOCP_SERVER\ClientPacketHandler.h"
#include "..\IOCP\Service.h"
#include "..\IOCP_SERVER\ClientSession.h"
#include "..\IOCP\ThreadManager.h"
#include "..\IOCP\IOCP.h"

shared_ptr<ClientSession> MakeSharedSesion()
{
    return make_shared<ClientSession>();
}

W::Application application;
shared_ptr<ServerService> GServerService;
atomic<bool> bIsRunning = true;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(41939);
    // TODO: 여기에 코드를 입력합니다.
    
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    GServerService = make_shared<ServerService>(NetAddress(L"127.0.0.1", 7777),
        make_shared<IOCP>(), MakeSharedSesion, 5);

    ClientPacketHandler::Initialize();

    GServerService->Start();

    for (int i = 0; i < 6; ++i)
    {
        ThreadMgr->Excute(
            [=]()
            {
                while (bIsRunning)
                {
                    GServerService->GetIOCP()->Excute();
                   
                }
            }
        );
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    // 기본 메시지 루프입니다:

    const int targetFPS = 120;
    const int frameDelayMs = 1000 / targetFPS; // 16ms

    while (true)
    {
        auto frameStart = std::chrono::high_resolution_clock::now();

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
             application.Run();
        }

        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = frameEnd - frameStart;

        int sleepTime = frameDelayMs - static_cast<int>(elapsed.count());

        if (sleepTime > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }


    W::ThreadPool::Shutdown();
    W::SceneManger::Release();

    ThreadMgr->Join();
    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;// MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 1400, 750, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    application.SetWindow(hWnd, 1400, 750);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    application.Initialize();
    W::InitializeScenes();
    
    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
    {
        bIsRunning.store(false);
        //GServerService->Stop();
        //프로토버퍼 종료
        google::protobuf::ShutdownProtobufLibrary();
        PostQuitMessage(0);
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
