#include "WStone.h"
#include "WResources.h"
#include "WStoneScript.h"
#include "WAnimator.h"
namespace W
{
	UINT Stone::CREATE_ID = 0;

	Stone::Stone()
	{
		
		AddComponent<StoneScript>();
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.8f, 0.8f));

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"stone_left", Vector2(0.0f, 0.0f), Vector2(69.0f, 106.0f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f);

		pAnim->Play(L"stone_left", true);

	}
	Stone::~Stone()
	{

	}

	void Stone::Initialize()
	{

	}

	void Stone::Update()
	{
		GameObject::Update();
	}
	void Stone::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	
}