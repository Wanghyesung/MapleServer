#include "WAven.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WAttackScript.h"
#include "WTime.h"
#include "WEventManager.h"
namespace W
{
	Aven::Aven() :
		m_iDir(1)
	{
		

		GetComponent<Transform>()->SetScale(2.f * 0.72, 2.f * 0.72, 0.f);

	}

	Aven::~Aven()
	{

	}

	void Aven::Initialize()
	{
		AttackScript* pScript = AddComponent<AttackScript>();
		pScript->SetStayObject(1, 10.f);

		Collider2D* pCollider = AddComponent<Collider2D>();
		GetComponent<Transform>()->SetScale(1.f, 1.f, 0.f);
		pCollider->SetSize(Vector2(0.5f, 0.5f));

	}

	void Aven::Update()
	{
		GameObject::Update();

		Vector3 vRotation = GetComponent<Transform>()->GetRotation();
		vRotation.z += XM_PI  * Time::DeltaTime() * 6;
		GetComponent<Transform>()->SetRotation(vRotation);
	}

	void Aven::LateUpdate()
	{
		Transform* pTr = GetComponent<Transform>();
		Vector3 vPosition = pTr->GetPosition();
		vPosition.x += (m_iDir * 8.f * Time::DeltaTime());
		pTr->SetPosition(vPosition);

		GameObject::LateUpdate();
	}

	void Aven::Off()
	{
		PlayerAttackObject::Off();
	}
}