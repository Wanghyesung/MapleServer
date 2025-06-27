#include "WGroggy.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WEventManager.h"
#include "WBattleManager.h"
namespace W
{
	UINT Groggy::CREATE_ID = 0;

	Groggy::Groggy()
	{
		
		GetComponent<Transform>()->SetScale(2.f, 2.f, 0.f);
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"groggy", Vector2(0.0f, 0.0f), Vector2(129.0f, 67.0f), 8, Vector2(139.f, 130.f), Vector2::Zero, 0.2f);

		pAnim->Play(L"groggy", true);

	}
	Groggy::~Groggy()
	{

	}
	void Groggy::Initialize()
	{
		
	}
	void Groggy::Update()
	{
		GameObject::Update();
	}

	void Groggy::LateUpdate()
	{
		Abnormal::LateUpdate();
	}

	void Groggy::Restore()
	{
		EventManager::Restore(m_pTarget, BattleManager::eAbnormalType::InputKey);
	}

	void Groggy::UpdatePacket()
	{
		Abnormal::UpdatePacket();
	}
}