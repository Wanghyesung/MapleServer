#include "WCollider2D.h"
#include "WGameObject.h"
#include "WTransform.h"

namespace W
{
	UINT Collider2D::COMPONENT_ID = 0;
	UINT Collider2D::ColliderNumber = 0;
	Collider2D::Collider2D():
		Component(eComponentType::Collider2D),
		m_pTransform(nullptr),
		m_vSize(Vector2::One),
		m_vCenter(Vector2::Zero),
		m_bActive(true)
	{
		++ColliderNumber;
		m_iColliderID = ColliderNumber;
	}
	Collider2D::~Collider2D()
	{

	}
	void Collider2D::Initialize()
	{
		m_pTransform = GetOwner()->GetComponent<Transform>();
	}
	void Collider2D::Update()
	{
	}
	void Collider2D::LateUpdate()
	{
		Transform* pTransform = GetOwner()->GetComponent<Transform>();

		Vector3 vScale = pTransform->GetScale();
		vScale.x *= m_vSize.x;
		vScale.y *= m_vSize.y;

		Vector3 vPos = pTransform->GetPosition();
		vPos.x += m_vCenter.x;
		vPos.y += m_vCenter.y;

		m_vPosition = vPos;
	}
	void Collider2D::Render()
	{
	}
	void Collider2D::OnCollisionEnter(Collider2D* other)
	{
		Script* pScript = GetOwner()->GetScript<Script>();
		pScript->OnCollisionEnter(other);
		
	}
	void Collider2D::OnCollisionStay(Collider2D* other)
	{
		Script* pScript = GetOwner()->GetScript<Script>();
		pScript->OnCollisionStay(other);
		
	}
	void Collider2D::OnCollisionExit(Collider2D* other)
	{
		Script* pScript = GetOwner()->GetScript<Script>();
		pScript->OnCollisionExit(other);
		
	}
}