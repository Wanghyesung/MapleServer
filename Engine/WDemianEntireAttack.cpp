#include "WDemianEntireAttack.h"
#include "WResources.h"
#include "WBattleManager.h"
#include "WPlayer.h"
#include "WEventManager.h"
#include "WInput.h"
#include "WPlayerScript.h"
#include "WTime.h"
#include "WAnimator.h"

namespace W
{
	UINT DemianEntireAttack::CREATE_ID = 0;

	DemianEntireAttack::DemianEntireAttack():
		m_iKeyDonwCount(50),
		m_fDeleteTime(5.f),
		m_fCurTime(0.f),
		m_iCallStack(0)
	{
		
	}
	DemianEntireAttack::~DemianEntireAttack()
	{

	}
	void DemianEntireAttack::Initialize()
	{
		
	}
	void DemianEntireAttack::Update()
	{
		GameObject::Update();

		if (m_pTarget == nullptr)
			return;

		m_fCurTime += Time::DeltaTime();

		if (Input::GetKeyDown(m_pTarget->GetPlayerID(),eKeyCode::LEFT))
			--m_iKeyDonwCount;
		else if(Input::GetKeyDown(m_pTarget->GetPlayerID(),eKeyCode::RIGHT))
			--m_iKeyDonwCount;
		
		if (m_iKeyDonwCount <= 0)
		{
			Restore();
		}
		else
		{
			if (m_fCurTime >= m_fDeleteTime)
				Restore();
		}
	}

	void DemianEntireAttack::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void DemianEntireAttack::UpdatePacket()
	{

	}
	
	void DemianEntireAttack::Restore()
	{
		EventManager::Restore(m_pTarget, BattleManager::eAbnormalType::DemianStop);
		EventManager::DeleteObject(this);
	}


}