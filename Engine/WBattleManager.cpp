#include "WBattleManager.h"
#include "WResources.h"
#include "WSceneManger.h"
#include "WTime.h"
#include "WEventManager.h"
#include "WPlayerScript.h"
#include "WSkillManager.h"
#include "WTemptation.h"
#include "WMonsterScript.h"
#include "WMonster.h"
#include "WAttackUp.h"
#include "WDefenseUp.h"
#include "WSealSkill.h"
#include "WFaint.h"
#include "WIgnore.h"
#include "WAnimator.h"
#include "WReflex.h"
#include "WConfusion.h"
#include "WVariation.h"
#include "WSlow.h"
#include "WUndead.h"
#include "WGroggy.h"
#include "WStigma.h"
#include "WInputBackground.h"
#include "WDemianEntireAttack.h"
#include "WStop.h"


namespace W
{
#define EffectMap std::map<std::wstring, std::queue<Effect*>> 
#define FuncAbnormalArr std::function<void (GameObject* _pTarget, float _fAccValue)>
#define FuncStatArr std::function<void(GameObject* _pTarget, float _fAccValue)>

	FuncAbnormalArr BattleManager::m_arrAbnormalFunc[(UINT)eAbnormalType::End];
	FuncStatArr BattleManager::m_arrStatFunc[(UINT)eUpStatType::End];
	EffectMap BattleManager::m_mapEffects = {};

	UINT BattleManager::m_iMaxDamage = 9999999;
	bool BattleManager::m_bOnAbnormal = false;
	bool BattleManager::m_bOnUndead = false;
	float BattleManager::m_fPotionTime = 0.1f;
	float BattleManager::m_fCurPotionTime = 0.f;
	UINT  BattleManager::m_arrStigmaCount[6] = { 0,6,6,6,6,6 };

		
	void BattleManager::Initialize()
	{
		m_arrAbnormalFunc[(UINT)eAbnormalType::None] = nullptr;
		m_arrAbnormalFunc[(UINT)eAbnormalType::SealSkill] = seal_skill;
		m_arrAbnormalFunc[(UINT)eAbnormalType::temptation] = temptation;
		m_arrAbnormalFunc[(UINT)eAbnormalType::Faint] = faint;
		m_arrAbnormalFunc[(UINT)eAbnormalType::Confusion] = confusion;
		m_arrAbnormalFunc[(UINT)eAbnormalType::Slow] = debuff_slow;
		m_arrAbnormalFunc[(UINT)eAbnormalType::Stop] = stop;
		m_arrAbnormalFunc[(UINT)eAbnormalType::Undead] = undead;
		m_arrAbnormalFunc[(UINT)eAbnormalType::DemianStop] = demianstop;
		m_arrAbnormalFunc[(UINT)eAbnormalType::InputKey] = inputkey;
		m_arrAbnormalFunc[(UINT)eAbnormalType::Stigma] = stigma;
	
		m_arrStatFunc[(UINT)eUpStatType::None] = nullptr;
		m_arrStatFunc[(UINT)eUpStatType::Attack] = buff_attack;
		m_arrStatFunc[(UINT)eUpStatType::Speed] = buff_speed;
		m_arrStatFunc[(UINT)eUpStatType::Defense] = buff_defense;
		m_arrStatFunc[(UINT)eUpStatType::Heal] = buff_heal;
		m_arrStatFunc[(UINT)eUpStatType::Ignore] = buff_ignore;
		m_arrStatFunc[(UINT)eUpStatType::ReflexAttack] = buff_reflex;
		m_arrStatFunc[(UINT)eUpStatType::AccHP] = buff_HP;
		m_arrStatFunc[(UINT)eUpStatType::AccMP] = buff_MP;
	}

	void BattleManager::Release()
	{
		EffectMap::iterator iter = m_mapEffects.begin();
		for (iter; iter != m_mapEffects.end(); ++iter)
		{
			std::queue<Effect*> queuEffect = iter->second;
			
			while (!queuEffect.empty())
			{
				Effect* pEffect = queuEffect.front();
				queuEffect.pop();

				delete pEffect;
				pEffect = nullptr;
			}
		}
	}

	void BattleManager::Update()
	{

	}

	void BattleManager::CheckDamage(tObjectInfo& _tObjectInfo, const tAttackInfo& _tAttackInfo,
		const std::wstring _strName, Vector3 _vPosition)
	{
		int iFinalDamage = 0;

		iFinalDamage = floor(_tAttackInfo.fAttackDamage * 10
			- _tObjectInfo.fDefense * _tObjectInfo.fDefense);

		if (iFinalDamage <= 0.f)
			iFinalDamage = 1.f;

		else if (iFinalDamage >= m_iMaxDamage)
			iFinalDamage = m_iMaxDamage;

		_tObjectInfo.fHP -= iFinalDamage;
		if (_tObjectInfo.fHP <= 0.f)
			_tObjectInfo.fHP = 0.f;

	}

	void BattleManager::HitchAbnormal(GameObject* _pPlayer, eAbnormalType _eType, float _fAccStat)
	{
		if (_eType == eAbnormalType::None)
			return;

		if (_eType == eAbnormalType::Stigma)
		{
			//씬에서 관리하는 게 더 좋음
			EventManager::HitchAbnormal(_pPlayer, _eType, _fAccStat);
			return;
		}
			
		if (!m_bOnAbnormal)
			EventManager::HitchAbnormal(_pPlayer, _eType, _fAccStat);

		m_bOnAbnormal = true;
	}


	void BattleManager::excute_abnormal(eAbnormalType _eType, GameObject* _pTarget, float _fAccValue)
	{
		m_arrAbnormalFunc[(UINT)_eType](_pTarget, _fAccValue);
	}

	void BattleManager::excute_stat(eUpStatType _eType, GameObject* _pTarget, float _fAccValue)
	{
		m_arrStatFunc[(UINT)_eType](_pTarget, _fAccValue);
	}

	void BattleManager::stop(GameObject* _pGameObject, float _fAccValue)
	{
		Player* pPlayer = dynamic_cast<Player*>(_pGameObject);
		if (!pPlayer)
			return;

		Stop * pStop = new Stop();
		pStop->SetTarget(pPlayer);
		pStop->SetTime(6.f);
		EventManager::CreateObject(pStop, eLayerType::Object);

		PlayerScript* pScript = pPlayer->GetScript<PlayerScript>();
		pScript->m_bAbnormal = true;

		EventManager::ChangePlayerSkillState(pPlayer->GetPlayerID(), Player::ePlayerSkill::end);
		EventManager::ChangePlayerFSMState(pScript->m_pFSM, Player::ePlayerState::alert);
	}
	void BattleManager::faint(GameObject* _pGameObject, float _fAccValue)
	{
		Player* pPlayer = dynamic_cast<Player*>(_pGameObject);
		if (!pPlayer)
			return;

		Faint* pFaint = new Faint();
		pFaint->SetTarget(pPlayer);
		pFaint->SetTime(5.f);
		EventManager::CreateObject(pFaint, eLayerType::Object);

		PlayerScript* pScript = pPlayer->GetScript<PlayerScript>();
		pScript->m_bAbnormal = true;
		
		EventManager::ChangePlayerSkillState(pPlayer->GetPlayerID(), Player::ePlayerSkill::end);
		EventManager::ChangePlayerFSMState(pScript->m_pFSM, Player::ePlayerState::alert);
		//SkillManager::SetActiveSkill(Player::ePlayerSkill::end);
	}
	
	//음수로 넣어야함
	void BattleManager::debuff_slow(GameObject* _pGameObject, float _fAccSpeed)
	{
		Player* pPlayer = dynamic_cast<Player*>(_pGameObject);
		tObjectInfo& tInfo = pPlayer->GetScript<PlayerScript>()->m_tObjectInfo;
		if (!pPlayer)
			return;

		if (_fAccSpeed < 0)
		{
			Slow* pSlow = new Slow(_fAccSpeed);
			pSlow->SetTarget(pPlayer);
			pSlow->SetTime(6.f);
			EventManager::CreateObject(pSlow, eLayerType::Object);
		}

		tInfo.fSpeed += _fAccSpeed;
	}

	void BattleManager::seal_skill(GameObject* _pGameObject, float _fAccValue)
	{
		Player* pPlayer = dynamic_cast<Player*>(_pGameObject);
		if (!pPlayer)
			return;

		SealSkill* pSeal = new SealSkill();
		pSeal->SetTarget(pPlayer);
		pSeal->SetTime(7.f);
		EventManager::CreateObject(pSeal, eLayerType::Object);

		PlayerScript* pScript = pPlayer->GetScript<PlayerScript>();
		pScript->m_bSealSkill = true;

		EventManager::ChangePlayerSkillState(pPlayer->GetPlayerID(), Player::ePlayerSkill::end);
		//SkillManager::SetActiveSkill(Player::ePlayerSkill::end);
	}

	void BattleManager::temptation(GameObject* _pGameObject, float _fAccValue)
	{
		Player* pPlayer = dynamic_cast<Player*>(_pGameObject);
		if (!pPlayer)
			return;

		Temptation* pTemptation = new Temptation();
		pTemptation->SetTarget(pPlayer);
		pTemptation->SetTime(5.f);
		EventManager::CreateObject(pTemptation, eLayerType::Object);


		//스킬 초기화
	
		EventManager::ChangePlayerSkillState(pPlayer->GetPlayerID(), Player::ePlayerSkill::end);

		PlayerScript* pScript = pPlayer->GetScript<PlayerScript>();
		pScript->m_pFSM->SetActiveState(Player::ePlayerState::jump);
		pPlayer->SetPlayerCurState(Player::ePlayerState::jump);
		pPlayer->SetCurStateName(L"_jump");

		pScript->m_bAbnormal = true;

		srand(time(NULL));
		int i = rand() % 2;
		if (i == 0)
			pPlayer->SetDir(1);
		else
			pPlayer->SetDir(-1);
	}

	void BattleManager::undead(GameObject* _pGameObject, float _fAccValue)
	{
		Player* pPlayer = dynamic_cast<Player*>(_pGameObject);
		if (!pPlayer)
			return;

		Undead* pUndead = new Undead();
		pUndead->SetTarget(pPlayer);
		pUndead->SetTime(8.f);
		EventManager::CreateObject(pUndead, eLayerType::Object);

		m_bOnUndead = true;
	}

	void BattleManager::PushEffect(Effect* _pEffect)
	{
		
		EffectMap::iterator iter = m_mapEffects.find(_pEffect->GetName());
		if (iter == m_mapEffects.end())
		{
			std::queue<Effect*> pQueue;
			pQueue.push(_pEffect);
			m_mapEffects.insert(std::make_pair(_pEffect->GetName(), pQueue));
		}
		else
		{
			iter->second.push(_pEffect);
			_pEffect->SetActive(false);
		}

		_pEffect->SetPoolObject(true);
	}

	Effect* BattleManager::GetEffect(std::wstring _strName)
	{
		EffectMap::iterator iter = m_mapEffects.find(_strName);

		if (iter == m_mapEffects.end() ||
			iter->second.empty())
			return  nullptr;
		
		Effect* pEffect = iter->second.front();
		iter->second.pop();
		

		return pEffect;
	}

	void BattleManager::restore(GameObject* _pTarget, eAbnormalType _eType, float _fAccValue)
	{
		PlayerScript* pScript = _pTarget->GetScript<PlayerScript>();
		switch (_eType)
		{
		case W::BattleManager::eAbnormalType::SealSkill:
			pScript->m_bSealSkill = false;
			break;
		case W::BattleManager::eAbnormalType::temptation:
			pScript->m_bAbnormal = false;
			break;
		case W::BattleManager::eAbnormalType::Faint:
			pScript->m_bAbnormal = false;
			break;
		case W::BattleManager::eAbnormalType::Stop:
			pScript->m_bAbnormal = false;
			break;
		case W::BattleManager::eAbnormalType::Slow:
			debuff_slow(_pTarget, _fAccValue);
			break;
		case W::BattleManager::eAbnormalType::Undead:
			m_bOnUndead = false;
			break;
		case W::BattleManager::eAbnormalType::DemianStop:
			pScript->m_bAbnormal = false;
			break;
		case W::BattleManager::eAbnormalType::InputKey:
			pScript->m_bAbnormal = false;
			break;
		case W::BattleManager::eAbnormalType::Stigma:
			UINT iObjectID = _pTarget->GetObjectID();
			m_arrStigmaCount[iObjectID] = 0;
			break;
		}

		m_bOnAbnormal = false;
	}

	bool BattleManager::IsAblePotion()
	{
		if(m_fCurPotionTime>= m_fPotionTime)
			return true;

		return false;
	}

	void BattleManager::buff_attack(GameObject* _pTarget, float _fAccDamage)
	{
		Monster* pTarget=  dynamic_cast<Monster*>(_pTarget);
		MonsterScript* pScript = pTarget->GetScript<MonsterScript>();

		if (_fAccDamage > 0.f)
		{
			AttackUp* pAttackUp = new AttackUp(_fAccDamage);
			pAttackUp->SetSceneID(_pTarget->GetSceneID());
			pAttackUp->SetTarget(pTarget);
			pAttackUp->SetType(eUpStatType::Attack);
			pAttackUp->SetDeleteTime(10.f);
			EventManager::CreateObject(pAttackUp, eLayerType::Object);

		}

		//기본 공격
		pScript->m_tMonsterInfo.tAttackInfo.fAttackDamage += _fAccDamage;

		//스킬 공격
		for (UINT i = 0; i < pScript->m_vecAttacks.size(); ++i)
		{
			pScript->m_vecAttacks[i].tAttackInfo.fAttackDamage += _fAccDamage;
		}
		
	}

	void BattleManager::buff_defense(GameObject* _pTarget, float _fDefense)
	{
		Monster* pTarget = dynamic_cast<Monster*>(_pTarget);
		MonsterScript* pScript = pTarget->GetScript<MonsterScript>();

		if (_fDefense > 0.f)
		{
			DefenseUp* pAttackUp = new DefenseUp(_fDefense);
			pAttackUp->SetSceneID(_pTarget->GetSceneID());
			pAttackUp->SetTarget(pTarget);
			pAttackUp->SetType(eUpStatType::Defense);
			pAttackUp->SetDeleteTime(10.f);
			EventManager::CreateObject(pAttackUp, eLayerType::Object);

		}

		pScript->m_tObjectInfo.fDefense += _fDefense;
	}

	void BattleManager::buff_ignore(GameObject* _pTarget, float _fsign)
	{
		Monster* pTarget = dynamic_cast<Monster*>(_pTarget);
		MonsterScript* pScript = pTarget->GetScript<MonsterScript>();

		if (_fsign >=0.f)
		{
			Ignore* pIgnore = new Ignore(m_iMaxDamage);
			pIgnore->SetSceneID(_pTarget->GetSceneID());
			pIgnore->SetTarget(pTarget);
			pIgnore->SetType(eUpStatType::Ignore);
			pIgnore->SetDeleteTime(10.f);
			EventManager::CreateObject(pIgnore, eLayerType::Object);

			pScript->m_tObjectInfo.fDefense += m_iMaxDamage;
		}
		else
		{
			pScript->m_tObjectInfo.fDefense -= m_iMaxDamage;
		}
		
	}

	void BattleManager::confusion(GameObject* _pGameObject, float _fAccValue)
	{
		Player* pPlayer = dynamic_cast<Player*>(_pGameObject);
		if (!pPlayer)
			return;

		Confusion* pConfusion = new Confusion();
		pConfusion->SetSceneID(pPlayer->GetSceneID());
		pConfusion->SetTarget(pPlayer);
		pConfusion->SetTime(7.f);

		EventManager::CreateObject(pConfusion, eLayerType::Object);
	}
	void BattleManager::demianstop(GameObject* _pGameObject, float _fAccValue)
	{
		Player* pPlayer = static_cast<Player*>(_pGameObject);
		if (!pPlayer)
			return;

		DemianEntireAttack* DemianEnire = new DemianEntireAttack();
		DemianEnire->SetSceneID(pPlayer->GetSceneID());
		DemianEnire->SetTarget(pPlayer);
		 
		PlayerScript* pScript = pPlayer->GetScript<PlayerScript>();
		pScript->m_bAbnormal = true;

		EventManager::ChangePlayerSkillState(pPlayer->GetPlayerID(), Player::ePlayerSkill::end);
		EventManager::ChangePlayerFSMState(pScript->m_pFSM, Player::ePlayerState::alert);

		EventManager::CreateObject(DemianEnire, eLayerType::Object);
	}

	void BattleManager::inputkey(GameObject* _pGameObject, float _fAccValue)
	{
		Player* pPlayer = static_cast<Player*>(_pGameObject);
		if (!pPlayer)
			return;

		Groggy* pGroggy = new Groggy();
		pGroggy->SetTarget(pPlayer);
		pGroggy->SetSceneID(pPlayer->GetSceneID());
		pGroggy->SetTime(999999.f);
		//플레이어 아래에 그리기
		pGroggy->SetRenderPos(false);
		pGroggy->Initialize();

		PlayerScript* pScript = pPlayer->GetScript<PlayerScript>();
		pScript->m_bAbnormal = true;

		EventManager::ChangePlayerSkillState(pPlayer->GetPlayerID(), Player::ePlayerSkill::end);
		EventManager::ChangePlayerFSMState(pScript->m_pFSM, Player::ePlayerState::alert);

		//key입력
		InputBackground* pInput = new InputBackground();
		pInput->SetSceneID(pPlayer->GetSceneID());
		pInput->SetTarget(pPlayer);
		pInput->Initialize();
		pInput->SetOwner(pGroggy);

		EventManager::CreateObject(pInput, eLayerType::Object);
		EventManager::CreateObject(pGroggy, eLayerType::Object);
	}

	void BattleManager::stigma(GameObject* _pGameObject, float _fAccValue)
	{
		UINT iObjID = _pGameObject->GetObjectID();
		++m_arrStigmaCount[iObjID];

		if (m_arrStigmaCount[iObjID] >= 7)
		{
			tAttackInfo tAttackInfo = {};
			tAttackInfo.fAttack = m_iMaxDamage;
			tAttackInfo.fAttRcnt = 0.f;
			tAttackInfo.fAttUpperRcnt = 0.f;
			Player* pPlayer = static_cast<Player*>(_pGameObject);

			pPlayer->GetScript<PlayerScript>()->Hit(tAttackInfo, L"stigma");
			
			Stigma* pStigma = new Stigma();
			pStigma->SetSceneID(pPlayer->GetSceneID());
			pStigma->SetTarget(pPlayer);
			pStigma->SetTime(999.f);
			EventManager::CreateObject(pStigma, eLayerType::Object);
		}
	}

	void BattleManager::buff_heal(GameObject* _pTarget, float _fAccHeal)
	{
		Monster* pTarget = dynamic_cast<Monster*>(_pTarget);
		MonsterScript* pScript = pTarget->GetScript<MonsterScript>();

		pScript->m_tObjectInfo.fHP += _fAccHeal;
	}
	
	void BattleManager::buff_speed(GameObject* _pTarget, float _fAccSpeed)
	{
		Player* pTarget = dynamic_cast<Player*>(_pTarget);
		PlayerScript* pScript = pTarget->GetScript<PlayerScript>();

		if (_fAccSpeed >= 0.f)
		{
			UpStat* pStat = new UpStat(_fAccSpeed);
			pStat->SetTarget(pTarget);
			pStat->SetSceneID(pTarget->GetSceneID());
			pStat->SetDeleteTime(10.f);
			pStat->SetType(eUpStatType::Speed);
			EventManager::CreateObject(pStat, eLayerType::Object);
		}
		pScript->m_tObjectInfo.fSpeed += _fAccSpeed;
	}

	void BattleManager::buff_MP(GameObject* _pTarget, float _fAccValue)
	{
		tObjectInfo& tInfo = _pTarget->GetScript<PlayerScript>()->m_tObjectInfo;

		tInfo.fMP += _fAccValue;
		if (tInfo.fMP >= tInfo.fMaxMP)
			tInfo.fMP = tInfo.fMaxMP;
	}

	void BattleManager::buff_HP(GameObject* _pTarget, float _fAccValue)
	{
		tObjectInfo& tInfo = _pTarget->GetScript<PlayerScript>()->m_tObjectInfo;

		if (_fAccValue >= m_iMaxDamage)
			_fAccValue = tInfo.fMaxHP;

		if (m_bOnUndead)
			_fAccValue /= 2.f;

		tInfo.fHP += _fAccValue;
		if (tInfo.fHP >= tInfo.fMaxHP)
			tInfo.fHP = tInfo.fMaxHP;
	}

	void BattleManager::buff_reflex(GameObject* _pTarget, float _fsign)
	{
		Monster* pTarget = dynamic_cast<Monster*>(_pTarget);
		MonsterScript* pScript = pTarget->GetScript<MonsterScript>();

		if (_fsign >= 0.f)
		{
			Reflex* pReflex = new Reflex(m_iMaxDamage);
			pReflex->SetSceneID(pTarget->GetSceneID());
			pReflex->SetTarget(pTarget);
			pReflex->SetType(eUpStatType::ReflexAttack);
			pReflex->SetDeleteTime(10.f);
			
			EventManager::CreateObject(pReflex, eLayerType::Object);
			pScript->m_tObjectInfo.fDefense += m_iMaxDamage;

			pScript->m_bReflex = true;
		}
		else
		{
			pScript->m_tObjectInfo.fDefense -= m_iMaxDamage;
			pScript->m_bReflex = false;
		}
	}

	void BattleManager::Buff_Stat(GameObject* _pTarget, eUpStatType _eType, float _fAccStat)
	{
		EventManager::UpStat(_pTarget, _eType, _fAccStat);
	}

	void BattleManager::Player_DeBuff_Attack(GameObject* _pTarget, eUpStatType _eType, float _fAccStat)
	{
		//_eType 타입에 따라 추가 가능
		if (_fAccStat >= 0.f)
			_pTarget->GetScript<PlayerScript>()->m_tObjectInfo.fDefense / fabs(_fAccStat);
		else
			_pTarget->GetScript<PlayerScript>()->m_tObjectInfo.fDefense * fabs(_fAccStat);
	}


	
	
}