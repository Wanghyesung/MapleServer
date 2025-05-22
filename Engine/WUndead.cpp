#include "WUndead.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WBattleManager.h"
#include "WPlayer.h"
#include "WEventManager.h"
namespace W
{
	UINT Undead::CREATE_ID = 0;

	Undead::Undead()
	{
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"undead", Vector2(0.0f, 0.0f), Vector2(43.0f, 46.0f), 10, Vector2(70.f, 70.f), Vector2::Zero, 0.15f);

		pAnim->Play(L"undead", true);

		//m_eType = BattleManager::eAbnormalType::Undead;

	}
	Undead::~Undead()
	{
	}
	void Undead::Initialize()
	{
	}
	void Undead::Update()
	{
		Abnormal::Update();
	}
	void Undead::LateUpdate()
	{
		Abnormal::LateUpdate();

	}
	
	void Undead::Restore()
	{
		EventManager::Restore(m_pTarget, BattleManager::eAbnormalType::Undead);

	}
}