#include "WTemptation.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WRigidbody.h"
#include "WPlayer.h"
#include "WBattleManager.h"
#include "WEventManager.h"
namespace W
{
	UINT Temptation::CREATE_ID = 0;

	Temptation::Temptation()
	{
	
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"temptation", Vector2(0.0f, 0.0f), Vector2(50.0f, 63.0f), 8, Vector2(80.f, 80.f), Vector2::Zero, 0.2f);

		pAnim->Play(L"temptation", true);

	}
	Temptation::~Temptation()
	{

	}
	void Temptation::Initialize()
	{

	}
	void Temptation::Update()
	{
		Abnormal::Update();

		if (m_pTarget == nullptr)
			return;

		Rigidbody* pRigidbody = m_pTarget->GetComponent<Rigidbody>();
		int iDir = m_pTarget->GetDir();

		bool bGround = pRigidbody->IsGround();
		if (bGround)
		{
			pRigidbody->SetVelocity(Vector2(iDir * 1.5f, 3.5f));
			pRigidbody->SetGround(false);
		}
		else
			pRigidbody->AddForce(Vector2(1.5f * iDir, 0.f));
	}
	void Temptation::LateUpdate()
	{
		Abnormal::LateUpdate();
	}


	void Temptation::Restore()
	{
		EventManager::Restore(m_pTarget, BattleManager::eAbnormalType::temptation);
	}
	
	void Temptation::UpdatePacket()
	{
		Abnormal::UpdatePacket();
	}
}