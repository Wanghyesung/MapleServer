#include "WStigmaBack.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WSceneManger.h"
#include "WStigmaCount.h"
#include "WBattleManager.h"
#include "WEventManager.h"
namespace W
{
	StigmaBack::StigmaBack():
		m_iCount(0)
	{
		GetComponent<Transform>()->SetScale(1.5f, 1.5f, 0.f);


		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"loop", Vector2(0.0f, 0.0f), Vector2(80.0f, 43.0f), 11, Vector2(80.f, 80.f), Vector2::Zero, 0.13f);

		pAnim->Play(L"loop", true);
	}
	StigmaBack::~StigmaBack()
	{

	}
	void StigmaBack::Initialize()
	{
		m_pCount = new StigmaCount();
		m_pCount->SetSceneName(GetSceneName());
		m_pCount->set_count(0);
		m_pCount->m_pOwner = this;

		EventManager::CreateObject(m_pCount, eLayerType::Object);

	}
	void StigmaBack::Update()
	{
		set_count();

		m_pTarget = SceneManger::FindPlayer(GetSceneName());
		Vector3 vPosition =	m_pTarget->GetComponent<Transform>()->GetPosition();
		vPosition.y += 0.7f;
		vPosition.z = -2.f;
		GetComponent<Transform>()->SetPosition(vPosition);

		GameObject::Update();
	}
	void StigmaBack::LateUpdate()
	{
		if (m_iCount == 0)
			return;
		GameObject::LateUpdate();
	}
	
	void StigmaBack::set_count()
	{
		UINT iCount = BattleManager::GetStigmaCount();
		if (m_iCount != iCount)
		{
			m_iCount = iCount;
			m_pCount->set_count(m_iCount);
		}
	}
}