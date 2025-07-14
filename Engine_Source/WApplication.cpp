#include "WApplication.h"

#include "WTime.h"
#include "WInput.h"

#include "WSceneManger.h"
#include "WCollisionManager.h"
#include "..\Engine\WSkillManager.h"
#include "..\Engine\WBattleManager.h"
#include "..\Engine\WEventManager.h"
#include "..\Engine\WMonsterManager.h"
#include "..\Engine\WGameObjectManager.h"
#include "..\Engine\WItemManager.h"
#include "WPathManager.h"
#include "WThreadPool.h"
#include "WFmod.h"
#include "WFontWrapper.h"
namespace W
{
	//0,0의 해상도도 있음
	Application::Application():
		//:graphicDevice(nullptr),
		m_hHwnd(NULL),
		m_iWidth(-1),
		m_iHeight(-1)
	{

	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Input::Update();
		SkillManager::Update();
		Update();
		LateUpdate();
		UpdatePacket();

		EventManager::Update();
	}


	void Application::Initialize()
	{
		//ThreadPool::Initiailize(4);
		PathManager::Initialize();
		Time::Initiailize();
		Input::Initialize();
		
		CollisionManager::Initialize();
		BattleManager::Initialize();
		EventManager::Initialize();

		ItemManager::Initialize();
		GameObjectManager::Initialize();

		SceneManger::Initialize();
	}

	void Application::Update()
	{
		
		Time::Update();
		
		CollisionManager::Update();
		SceneManger::Update();
		BattleManager::Update();
		MonsterManager::Update();
		
	}

	void Application::LateUpdate()
	{
		SceneManger::LateUpdate();
	}

	void Application::UpdatePacket()
	{
		SceneManger::UpdatePacket();
	}


	void Application::SetWindow(HWND _hHwnd, UINT _iWidth, UINT _iHeight)
	{
		//한번만 호출되게
		
		RECT rt = { 0, 0, (LONG)_iWidth , (LONG)_iHeight };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(m_hHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(m_hHwnd, true);
		UpdateWindow(m_hHwnd);
	}
}