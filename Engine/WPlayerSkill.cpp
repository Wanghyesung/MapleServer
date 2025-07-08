#include "WPlayerSkill.h"
#include "WSkillManager.h"
#include "WSkillState.h"
#include "WPlayerScript.h"
#include "WSkillLuck.h"
#include "WSkillDark.h"
#include "WSkillJump.h"
#include "WSkillSpeed.h"
#include "WSkillAven.h"
#include "WSkillShadow.h"
#include "WSkillQuad.h"
#include "WSkillRaid.h"
#include "WSkillWind.h"
#include "WSkillBlast.h"
#include "WSkillLoad.h"
#include "WPlayerScript.h"
#include "WUltimateSkill.h"
#include "WEventManager.h"


namespace W
{
	PlayerSkill::PlayerSkill():
		m_pActiveSkill(nullptr),
		m_bSkillOn(false)
	{
	}

	PlayerSkill::~PlayerSkill()
	{
		EventManager::ReleasePlayerSkill(m_pPlayer->GetPlayerID());

		std::map<Player::ePlayerSkill, SkillState*>::iterator iter =
			m_mapSkills.begin();

		for (iter; iter != m_mapSkills.end(); ++iter)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}

	void PlayerSkill::Update()
	{
		if (m_pActiveSkill)
			m_pActiveSkill->Update();
	}

	void PlayerSkill::Initialize()
	{
		AddSkill(new SkillLuck());
		AddSkill(new SkillDark());
		AddSkill(new SkillJump());
		AddSkill(new SkillSpeed());
		AddSkill(new SkillAven());
		AddSkill(new SkillShadow());
		AddSkill(new SkillQuad());
		AddSkill(new SkillRaid());
		AddSkill(new SkillWind());
		AddSkill(new SkillBlast());
		AddSkill(new SkillLoad());
		AddSkill(new SkillUltimate());
	}

	void PlayerSkill::RegisterSkill()
	{
		UINT iPlayerID = m_pPlayer->GetPlayerID();
		EventManager::InitPlayerSkill(iPlayerID, this);

		auto iter = m_mapSkills.begin();
		for(iter; iter!=m_mapSkills.end(); ++iter)
			EventManager::AddPlayerSkillState(iter->second);
	}

	SkillState* PlayerSkill::FindSkillState(Player::ePlayerSkill _eSkill)
	{
		std::map<Player::ePlayerSkill, SkillState*>::iterator iter =
			m_mapSkills.find(_eSkill);

		if (iter != m_mapSkills.end())
			return iter->second;

		return nullptr;
	}

	void PlayerSkill::AccAttack(Vector3 _vPosition)
	{
		PlayerScript* pScript = m_pPlayer->GetScript<PlayerScript>();

		SkillState* pState = FindSkillState(Player::ePlayerSkill::blast);
		SkillBlast* pBlast = static_cast<SkillBlast*>(pState);
		_vPosition.z -= 1.f;
		pBlast->create_blast(_vPosition);
		pScript->MinusAttackCnt();
	}

	void PlayerSkill::AddSkill(SkillState* _pSkill)
	{
		Player::ePlayerSkill eSkill = _pSkill->GetPlayerSkill();

		SkillState* pSkill = FindSkillState(eSkill);
		//똑같은 스킬

		if (pSkill)
			assert(nullptr);

		_pSkill->SetOnwer(this);
		_pSkill->Initialize();
		m_mapSkills.insert(std::make_pair(eSkill, _pSkill));
	}

	const wstring& PlayerSkill::GetCurSkillName()
	{
		if (m_pActiveSkill == nullptr)
			return {};

		return m_pActiveSkill->GetStateName();
	}

}