#include "WStigma.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WEventManager.h"

namespace W
{
	UINT Stigma::CREATE_ID = 0;

	Stigma::Stigma()
	{
		GetComponent<Transform>()->SetScale(2.5f, 2.5f, 0.f);

	

		Animator* pAnim = AddComponent<Animator>();
		//std::shared_ptr<Texture> pAtlas = Resources::Find<Texture>(L"Stigma");
		pAnim->Create(L"start", Vector2(0.0f, 0.0f), Vector2(189.0f, 187.0f), 7, Vector2(200.f, 200.f), Vector2::Zero, 0.12f);
		
		pAnim->CompleteEvent(L"start") = std::bind(&Stigma::Restore, this);
		//pAnim->CompleteEvent(L"stone_end") = std::bind(&MegnusStone::off, this);
		//mr->SetMaterial(pMater);
		pAnim->Play(L"start", true);
	}
	Stigma::~Stigma()
	{

	}
	void Stigma::Initialize()
	{

	}
	void Stigma::Update()
	{
		GameObject::Update();
	}

	void Stigma::LateUpdate()
	{
		Abnormal::LateUpdate();

		Vector3 vTargetPos = GetComponent<Transform>()->GetPosition();
		vTargetPos.y -= 0.5f;
		GetComponent<Transform>()->SetPosition(vTargetPos);
	}

	//void Stigma::Render()
	//{
	//	GameObject::Render();
	//}
	void Stigma::Restore()
	{
		
		EventManager::Restore(m_pTarget, BattleManager::eAbnormalType::Stigma);
		EventManager::DeleteObject(this);
		//BattleManager::Restore_move(m_pTarget, BattleManager::eAbnormalType::SealSkill);
	}
}