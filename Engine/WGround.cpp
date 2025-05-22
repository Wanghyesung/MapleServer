#include "WGround.h"
#include "WCollider2D.h"
#include "WGroundScript.h"

namespace W
{
	UINT Ground::CREATE_ID = 0;

	Ground::Ground(bool _bFinalGround)
	{
		//�⺻������ �浹ü ������
		AddComponent<Collider2D>();
		GroundScript* pScript = AddComponent<GroundScript>();
		if (_bFinalGround)
			pScript->SetFinalGround();
	}
	Ground::~Ground()
	{

	}
	void Ground::Initialize()
	{
	}
	void Ground::Update()
	{
		GameObject::Update();
	}
	void Ground::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	//void Ground::Render()
	//{
	//	GameObject::Render();
	//}
}