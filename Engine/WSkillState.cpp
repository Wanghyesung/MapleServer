#include "WSkillState.h"
#include "WPlayerSkill.h"
#include "WSceneManger.h"
#include "WEffect.h"
#include "WPlayerScript.h"
#include "WTime.h"
#include "WSkillManager.h"
#include "WPlayerAttackObject.h"
#include "WAttackScript.h"
#include "WBattleManager.h"
#include "WEventManager.h"
#include "WResources.h"
namespace W
{
	SkillState::SkillState():
		m_pOwner(nullptr),
		m_fAddDamage(10.f)
	{
	
	}

	SkillState::~SkillState()
	{
		//Fmod::SoundPlay()
	}

	void SkillState::Enter()
	{
		if (GetPlayer()->IsDark())
			GetPlayer()->SetDark(false);

		Player* pPlayer = GetPlayer();
		pPlayer->GetScript<PlayerScript>()->SetActiveState(Player::ePlayerState::stand);

		GetPlayer()->SetCurStateName(GetStateName());
		m_tSKillTime.fCurActiveTime = 0.f;

	}

	void SkillState::Exit()
	{
		//SkillManager::SetActiveSkill(Player::ePlayerSkill::end);
		EventManager::ChangePlayerSkillState(GetPlayer()->GetPlayerID(), Player::ePlayerSkill::end);

		Player::ePlayerState eState = GetPlayer()->GetCurPlayerState();
		if(eState != Player::ePlayerState::ladder)
			m_pOwner->GetPlayer()->SetCurStateName(L"_stand");
		else if(eState == Player::ePlayerState::ladder)
			m_pOwner->GetPlayer()->SetCurStateName(L"_ladder");
	}
	void SkillState::Update()
	{
		m_tSKillTime.fCurActiveTime += Time::DeltaTime();
	}

	void SkillState::Initialize()
	{

	}

	Player* SkillState::GetPlayer()
	{
		return m_pOwner->GetPlayer();
	}

	
	Effect* SkillState::CreateEffet(std::wstring _strName, Vector2 _vLeftTop, Vector2 _vSize,
		UINT _iColumnLength, UINT _iRowLength, Vector2 _vDivisionSize, Vector2 _vOffset, float _fDuration)
	{
		Effect* pEffect = new Effect();
		pEffect->SetName(_strName);
		pEffect->CreateAnimation(_vLeftTop, _vSize, _iColumnLength, _iRowLength, _vDivisionSize, _vOffset, _fDuration);

		return pEffect;
	}

	void SkillState::StartEffect(Effect* _pEffect)
	{
		Player* pPlayer = GetPlayer();
		_pEffect->SetOwner(pPlayer);
		_pEffect->SetActive(true);

		_pEffect->StartEffect(pPlayer->GetDir());
		_pEffect->SetSceneName(pPlayer->GetSceneName());

		EventManager::CreateObject(_pEffect, eLayerType::Effect);
	}
	void SkillState::AddDamage(PlayerAttackObject* _pAttackObject)
	{
		tAttackInfo& attack = _pAttackObject->GetScript<AttackScript>()->GetAttackInfo();
		attack.fAttackDamage *= m_fAddDamage;

		 //attack.fAttackDamage += 
	}
}