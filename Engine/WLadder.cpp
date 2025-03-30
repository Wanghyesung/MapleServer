#include "WLadder.h"
#include "WCollider2D.h"
#include "WLadderScript.h"
#include "WCollider2D.h"

namespace W
{
	Ladder::Ladder()
	{
		Collider2D* pDcollider = AddComponent<Collider2D>();
		AddComponent<LadderScript>();
	}

	Ladder::~Ladder()
	{

	}

	void Ladder::Initialize()
	{
		Collider2D* pDcollider = GetComponent<Collider2D>();
		Vector3 vScale = GetComponent<Transform>()->GetScale();
		pDcollider->SetSize(Vector2(0.3f, 1.f));
	
		GetScript<LadderScript>()->Initialize();
	}

	void Ladder::Update()
	{
		GameObject::Update();
	}

	void Ladder::LateUpdate()
	{
		GameObject::LateUpdate();
	}

}