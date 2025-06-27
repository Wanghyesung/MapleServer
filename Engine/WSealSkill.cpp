#include "WSealSkill.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WRigidbody.h"
#include "WPlayer.h"
#include "WBattleManager.h"
#include "WPlayer.h"
#include "WEventManager.h"
namespace W
{
	UINT SealSkill::CREATE_ID = 0;

	SealSkill::SealSkill()
	{
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"sealskill", Vector2(0.0f, 0.0f), Vector2(73.0f, 47.0f), 10, Vector2(80.f, 80.f), Vector2::Zero, 0.15f);

		pAnim->Play(L"sealskill", true);
	}
	SealSkill::~SealSkill()
	{

	}
	void SealSkill::Initialize()
	{

	}
	void SealSkill::Update()
	{
		Abnormal::Update();
	}
	void SealSkill::LateUpdate()
	{
		Abnormal::LateUpdate();
	}
	
	void SealSkill::Restore()
	{
		EventManager::Restore(m_pTarget, BattleManager::eAbnormalType::SealSkill);
	}

	void SealSkill::UpdatePacket()
	{
		Abnormal::UpdatePacket();
	}

}