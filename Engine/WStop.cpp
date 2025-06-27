#include "WStop.h"
#include "WResources.h"
#include "WBattleManager.h"
#include "WPlayer.h"
#include "WEventManager.h"
namespace W
{
	UINT Stop::CREATE_ID = 0;

	Stop::Stop()
	{
		
	}
	Stop::~Stop()
	{

	}
	void Stop::Initialize()
	{

	}
	void Stop::Update()
	{
		Abnormal::Update();
	}
	void Stop::LateUpdate()
	{
		Abnormal::LateUpdate();

	}

	void Stop::Restore()
	{
		EventManager::Restore(m_pTarget, BattleManager::eAbnormalType::Stop);

		//BattleManager::Restore_move(m_pTarget, BattleManager::eAbnormalType::Stop);
	}
}