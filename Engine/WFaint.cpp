#include "WFaint.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WRigidbody.h"
#include "WPlayer.h"
#include "WBattleManager.h"
#include "WEventManager.h"
namespace W
{
	Faint::Faint()
	{
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"faintSkill", Vector2(0.0f, 0.0f), Vector2(66.0f, 18.0f), 3, Vector2(70.f, 70.f), Vector2::Zero, 0.3f);

		pAnim->Play(L"faintSkill", true);
	}
	Faint::~Faint()
	{

	}
	void Faint::Initialize()
	{

	}
	void Faint::Update()
	{
		Abnormal::Update();
	}
	void Faint::LateUpdate()
	{
		Abnormal::LateUpdate();
	}

	void Faint::Restore()
	{
		EventManager::Restore(m_pTarget, BattleManager::eAbnormalType::Faint);

	}


}