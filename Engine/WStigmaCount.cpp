#include "WStigmaCount.h"
#include "WResources.h"
#include "WAnimator.h"
namespace W
{
	UINT StigmaCount::CREATE_ID = 0;

	StigmaCount::StigmaCount():
		m_iNumber(0),
		m_pOwner(nullptr)
	{
		GetComponent<Transform>()->SetScale(0.14f * 1.4f, 0.16f * 1.4f, 0.f);
		
		
	}
	StigmaCount::~StigmaCount()
	{

	}
	void StigmaCount::Initialize()
	{
		
	}
	void StigmaCount::Update()
	{
		if (m_iNumber == 0)
			return;

		Vector3 vPosition =
			m_pOwner->GetComponent<Transform>()->GetPosition();
		vPosition.z = -2.2f;
		vPosition.x -= 0.04f;
		vPosition.y -= 0.02f;

		GetComponent<Transform>()->SetPosition(vPosition);

		GameObject::Update();
	}
	void StigmaCount::LateUpdate()
	{
		if (m_iNumber == 0)
			return;
		GameObject::LateUpdate();
	}
	
	void StigmaCount::set_count(UINT _iCount)
	{
		if (_iCount>7)
			return;

		m_iNumber = _iCount;
	
	}
}