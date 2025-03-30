#include "WPortal.h"
#include "WPortalScript.h"
#include "WResources.h"
#include "WAnimator.h"

namespace W
{
	Portal::Portal()
	{
		AddComponent<PortalScript>();
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.8f, 0.8f));

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"portal0_left", Vector2(0.0f, 0.0f), Vector2(104.0f, 142.0f), 8, Vector2(120.f, 120.f),Vector2::Zero,0.2f);
		
		pAnim->Play(L"portal0_left", true);

		GetComponent<Transform>()->SetScale(Vector3(1.2f, 1.2f, 0.f));
	}
	Portal::~Portal()
	{

	}
	void Portal::Initialize()
	{

	}
	void Portal::Update()
	{
		GameObject::Update();
	}
	void Portal::LateUpdate()
	{
		GameObject::LateUpdate();
	}

}