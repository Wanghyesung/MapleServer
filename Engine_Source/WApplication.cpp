#include "WApplication.h"

#include "WTime.h"
#include "WInput.h"
#include "WRenderer.h"
#include "WSceneManger.h"
#include "UIManger.h"
#include "WCollisionManager.h"
#include "..\Engine\WSkillManager.h"
#include "..\Engine\WBattleManager.h"
#include "..\Engine\WEventManager.h"
#include "..\Engine\WMonsterManager.h"
#include "..\Engine\WItemManager.h"
#include "..\Engine_Source\WThreadPool.h"
#include "WFmod.h"
#include "WFontWrapper.h"
namespace W
{
	//0,0�� �ػ󵵵� ����
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
		SkillManager::Update();
		Update();
		LateUpdate();
		
		EventManager::Update();
		Destroy();
		UIManger::ReleaseChildUI();
	}


	void Application::Initialize()
	{
		ThreadPool::Initiailize(4);
		Time::Initiailize();
		Input::Initialize();
		

		BattleManager::Initialize();
		ItemManager::Initialize();
	}

	void Application::Update()
	{
		Time::Update();
		Input::Update();//�̰� ����

		CollisionManager::Update();
		SceneManger::Update();
		BattleManager::Update();
		UIManger::Update();
		MonsterManager::Update();
		
	}

	void Application::LateUpdate()
	{
		SceneManger::LateUpdate();
	}


	void Application::Destroy()
	{
		SceneManger::Destroy();
	}


	void Application::SetWindow(HWND _hHwnd, UINT _iWidth, UINT _iHeight)
	{
		//�ѹ��� ȣ��ǰ�
		
		RECT rt = { 0, 0, (LONG)_iWidth , (LONG)_iHeight };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(m_hHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(m_hHwnd, true);
		UpdateWindow(m_hHwnd);
	}
}