#include "WMonsterBackHP.h"
#include "WMonsterHP.h"
#include "WResources.h"

namespace W
{
	UINT MonsterBackHP::CREATE_ID = 0;

	MonsterBackHP::MonsterBackHP(bool _bBoss):
		m_pHP(nullptr)
	{
		if (!_bBoss)
		{
			GetComponent<Transform>()->SetScale(1.f * 0.7f, 1.f * 0.13f, 0.f);
		}
		else
		{
			GetComponent<Transform>()->SetScale(1.2f * 8.01f, 1.2f * 0.4f, 0.f);
		}
		
		
	}
	MonsterBackHP::~MonsterBackHP()
	{

	}
	void MonsterBackHP::Initialize()
	{

	}
	void MonsterBackHP::Update()
	{
		if (!m_pHP->IsActive())
			return;

		Vector3 vPosition = m_pHP->GetComponent<Transform>()->GetPosition();
		vPosition.z += 0.01f;
		if(m_pHP->IsBoss())
			vPosition.x -= 0.2f;

		GetComponent<Transform>()->SetPosition(vPosition);

		GameObject::Update();
	}
	void MonsterBackHP::LateUpdate()
	{
		if (!m_pHP->IsActive())
			return;

		GameObject::LateUpdate();
	}
	
}

