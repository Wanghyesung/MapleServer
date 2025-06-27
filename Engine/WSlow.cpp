#include "WSlow.h"
#include "WAnimator.h"
#include "WResources.h"
#include "WBattleManager.h"
#include "WPlayer.h"
#include "WEventManager.h"
namespace W
{
	UINT Slow::CREATE_ID = 0;

	Slow::Slow(float _fAccStat):
		m_fAccValue(_fAccStat)
	{
		
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"slow", Vector2(0.0f, 0.0f), Vector2(112.0f, 91.0f), 6, Vector2(110.f, 110.f), Vector2::Zero, 0.15f);

		pAnim->Play(L"slow", true);
	}
	Slow::~Slow()
	{

	}
	void Slow::Initialize()
	{
		
	}
	void Slow::Update()
	{
		Abnormal::Update();
	}
	void Slow::LateUpdate()
	{
		Abnormal::LateUpdate();

	}
	
	void Slow::Restore()
	{
		EventManager::Restore(m_pTarget, BattleManager::eAbnormalType::Slow, -m_fAccValue);
	}
	void Slow::UpdatePacket()
	{
		Abnormal::UpdatePacket();
	}
}