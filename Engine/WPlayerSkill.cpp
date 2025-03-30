#include "WPlayerSkill.h"
#include "WSkillManager.h"
#include "WSkillState.h"



namespace W
{
	PlayerSkill::PlayerSkill():
		m_pActiveSkill(nullptr),
		m_bSkillOn(false)
	{
	}

	PlayerSkill::~PlayerSkill()
	{
		SkillManager::Release(m_pPlayer->GetPlayerID());
	}

	void PlayerSkill::Update()
	{
		if (m_pActiveSkill)
			m_pActiveSkill->Update();
	}

	void PlayerSkill::Initialize()
	{
		UINT iPlayerID = m_pPlayer->GetPlayerID();
		SkillManager::InitPlayerSkill(iPlayerID, this);
	
	}

}