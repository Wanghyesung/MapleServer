#include "WSharHP.h"
#include "WResources.h"
#include "WMonsterBackHP.h"
#include "WSceneManger.h"
#include "WEventManager.h"

namespace W
{
	
	SharHP::SharHP():
		MonsterHP(true),
		m_fHP(100.f)
	{
	
	}
	SharHP::~SharHP()
	{
	}
	void SharHP::Initialize()
	{
		//보스용으로 만들기
		m_pHPBack = new MonsterBackHP(true);
		m_pHPBack->SetSceneName(GetSceneName());
		m_pHPBack->SetHP(this);
		EventManager::CreateObject(m_pHPBack, eLayerType::UI);
	}
	void SharHP::Update()
	{
		if (!IsActive())
			return;

		if (m_fHP <= 0.f)
		{
			EventManager::DeleteObject(this);
			EventManager::DeleteObject(m_pHPBack);
			return;
		}

		GameObject::Update();
	}
	void SharHP::LateUpdate()
	{
		if (!IsActive())
			return;

		GameObject::LateUpdate();
	}
	
}