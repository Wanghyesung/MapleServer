#include "WSkillManager.h"
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
#include "WTime.h"
namespace W
{
	std::vector<PlayerSkill*> SkillManager::m_vecSkill = {};
	std::vector<std::map<Player::ePlayerSkill, SkillState*>> SkillManager::m_vecSkillState = {};
	std::vector<std::map<Player::ePlayerSkill, tSkillTime>> SkillManager::m_vecSkillTime = {};
	
	std::queue<UINT> SkillManager::m_queueNewPlayer = {};
	std::queue<UINT> SkillManager::m_queueDeletePlayer = {};

	//PlayerSkill* SkillManager::m_pPlayerSkill = nullptr;

	//플레이어꺼 쓰기
	void SkillManager::Update()
	{
		InitPlayer();
		DeletePlayer();

		update_skill();
	}


	void SkillManager::InitPlayerSkill(UINT _iPlayerID, PlayerSkill* _pSkill)
	{
		if (m_vecSkillState.size() > _iPlayerID)
		{
			m_vecSkillState.resize(_iPlayerID + 1);
			m_vecSkillTime.resize(_iPlayerID + 1);
			m_vecSkill.resize(_iPlayerID + 1);
		}

		m_vecSkill[_iPlayerID] = _pSkill;
		m_queueNewPlayer.push(_iPlayerID);
	}


	void SkillManager::InitPlayer()
	{
		while (!m_queueNewPlayer.empty())
		{
			UINT iPlayerID = m_queueNewPlayer.front();
			m_queueNewPlayer.pop();

			AddSkill(iPlayerID, new SkillLuck());
			AddSkill(iPlayerID, new SkillDark());
			AddSkill(iPlayerID, new SkillJump());
			AddSkill(iPlayerID, new SkillSpeed());
			AddSkill(iPlayerID, new SkillAven());
			AddSkill(iPlayerID, new SkillShadow());
			AddSkill(iPlayerID, new SkillQuad());
			AddSkill(iPlayerID, new SkillRaid());
			AddSkill(iPlayerID, new SkillWind());
			AddSkill(iPlayerID, new SkillBlast());
			AddSkill(iPlayerID, new SkillLoad());
			AddSkill(iPlayerID, new SkillUltimate());
		}
	}

	void SkillManager::DeletePlayer()
	{
		while (m_queueDeletePlayer.empty())
		{
			UINT iPlayerID = m_queueDeletePlayer.front();
			m_queueDeletePlayer.pop();

			for (auto& iter : m_vecSkillState[iPlayerID])
			{
				delete iter.second;
				iter.second = nullptr;
			}

			m_vecSkillState.erase(m_vecSkillState.begin() + iPlayerID);
			m_vecSkill.erase(m_vecSkill.begin() + iPlayerID);
			m_vecSkillTime.erase(m_vecSkillTime.begin() + iPlayerID);
		}
	}

	void SkillManager::AddSkill(UINT _iPlayerID, SkillState* _pSkill)
	{
		Player::ePlayerSkill eSkill = _pSkill->GetPlayerSkill();
		SkillState* pSkill = FindSkillState(_iPlayerID, eSkill);
		//똑같은 스킬

		if (pSkill)
			assert(nullptr);
		
		_pSkill->SetOnwer(m_vecSkill[_iPlayerID]);
		_pSkill->Initialize();

		m_vecSkillState[_iPlayerID].insert(std::make_pair(eSkill, _pSkill));
		m_vecSkillTime[_iPlayerID].insert(std::make_pair(eSkill, _pSkill->GetCoolTime()));
		
	}

	//전부 서베에서 순차적으로 체크 클라에서 동시에 접근할 일 업음
	void SkillManager::SetActiveSkill(UINT _iPlayerID, Player::ePlayerSkill _eSkill)
	{
		PlayerSkill* pPlayerSkill = m_vecSkill[_iPlayerID];
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

		pPlayerSkill->m_pActiveSkill = FindSkillState(_iPlayerID, _eSkill);

		pPlayerSkill->m_bSkillOn = true;
		pPlayerSkill->m_pActiveSkill->Enter();
		set_max(_iPlayerID, _eSkill);
	}

	SkillState* SkillManager::FindSkillState(UINT _iPlayerID, Player::ePlayerSkill _eSkill)
	{
		std::map<Player::ePlayerSkill, SkillState*>& mapPlayerSkill = GetPlayerSkill(_iPlayerID);

		const auto& iter = mapPlayerSkill.find(_eSkill);
		if (iter != mapPlayerSkill.end())
			return iter->second;

		return nullptr;
	}

	std::map<Player::ePlayerSkill, SkillState*>& SkillManager::GetPlayerSkill(UINT _iPlayerID)
	{
		return m_vecSkillState[_iPlayerID];
	}

	void SkillManager::AccAttack(UINT _iPlayerID, math::Vector3 _vPosition)
	{
		SkillState* pState = FindSkillState(_iPlayerID, Player::ePlayerSkill::blast);
		SkillBlast* pBlast = dynamic_cast<SkillBlast*>(pState);

		PlayerScript* pScript = m_vecSkill[_iPlayerID]->m_pPlayer->GetScript<PlayerScript>();
		_vPosition.z -= 1.f;
		pBlast->create_blast(_vPosition);
		pScript->MinusAttackCnt();
	
	}
	void SkillManager::Release(UINT _iPlayerID)
	{
		m_queueDeletePlayer.push(_iPlayerID);
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