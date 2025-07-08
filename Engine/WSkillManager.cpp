#include "WSkillManager.h"
#include "WTime.h"
#include "WPlayerScript.h"
namespace W
{
	std::vector<PlayerSkill*> SkillManager::m_vecSkill = {};
	std::vector<std::map<Player::ePlayerSkill, tSkillTime>> SkillManager::m_vecSkillTime = {};
	
	
	//PlayerSkill* SkillManager::m_pPlayerSkill = nullptr;

	//플레이어꺼 쓰기
	void SkillManager::Update()
	{
		update_skill();
	}


	void SkillManager::InitSkill(UINT _iPlayerID, PlayerSkill* _pSkill)
	{
		if (_iPlayerID >= m_vecSkill.size())
		{
			m_vecSkill.resize(_iPlayerID + 1);
			m_vecSkillTime.resize(_iPlayerID + 1);
		}
		m_vecSkill[_iPlayerID] = _pSkill;
	}

	void SkillManager::ReleaseSkill(UINT _iPlayerID)
	{
		m_vecSkill.erase(m_vecSkill.begin() + _iPlayerID);
		m_vecSkillTime[_iPlayerID].clear();
	}


	void SkillManager::AddSkill(UINT _iPlayerID, SkillState* _pSkill)
	{
		Player::ePlayerSkill eSkill = _pSkill->GetPlayerSkill();
		m_vecSkillTime[_iPlayerID].insert(std::make_pair(eSkill, _pSkill->GetCoolTime()));
	}

	//전부 서베에서 순차적으로 체크 클라에서 동시에 접근할 일 업음
	void SkillManager::SetActiveSkill(UINT _iPlayerID, Player::ePlayerSkill _eSkill)
	{
		PlayerSkill* pPlayerSkill = FindPlayerSkill(_iPlayerID);
		PlayerScript* pScript = pPlayerSkill->m_pPlayer->GetScript<PlayerScript>();
		
		if (_eSkill == Player::ePlayerSkill::end)
		{
			pPlayerSkill->m_pActiveSkill = nullptr;
			pPlayerSkill->m_bSkillOn = false;
			return;
		}

		if (pPlayerSkill->m_pActiveSkill != nullptr)
			return;

		if (!check_cooltime(_iPlayerID, _eSkill))
			return;

		if (pScript->IsAbnormal() || pScript->IsAttack() || pScript->IsSealSkill())
			return;

		pPlayerSkill->m_pActiveSkill = pPlayerSkill->FindSkillState(_eSkill);

		pPlayerSkill->m_bSkillOn = true;
		pPlayerSkill->m_pActiveSkill->Enter();
		set_max(_iPlayerID, _eSkill);
	}

	//순사적으로 진입
	void SkillManager::AccAttack(UINT _iPlayerID, Vector3 _vPosition)
	{
		PlayerSkill* pPlayerSkill = FindPlayerSkill(_iPlayerID);
		pPlayerSkill->AccAttack(_vPosition);
	}


	void SkillManager::update_skill()
	{
		for (int i = 0; i < m_vecSkillTime.size(); ++i)
		{
			for (auto& iter : m_vecSkillTime[i])
			{
				tSkillTime& tTime = iter.second;
				if (tTime.fCurTime > 0.f)
				{
					tTime.fCurTime -= Time::DeltaTime();
				}
			}
		}
		
	}
	void SkillManager::set_max(UINT _iPlayerID, Player::ePlayerSkill _eTarget)
	{
		const auto& iter = m_vecSkillTime[_iPlayerID].find(_eTarget);

		tSkillTime& tTime = iter->second;
		tTime.fCurTime = tTime.fCoolTime;
	}

	bool SkillManager::check_cooltime(UINT _iPlayerID, Player::ePlayerSkill _eTarget)
	{
		const auto& iter = m_vecSkillTime[_iPlayerID].find(_eTarget);
		
		const tSkillTime& tTime = iter->second;
		if (tTime.fCurTime <= 0.f)
			return true;

		return false;
	}

}