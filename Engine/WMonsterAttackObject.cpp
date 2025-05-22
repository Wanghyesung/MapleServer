#include "WMonsterAttackObject.h"
#include "WMonsterAttackScript.h"
#include "WMonster.h"
#include "WMonsterScript.h"
#include "WEffect.h"

namespace W
{
	UINT MonsterAttackObject::CREATE_ID = 0;

	MonsterAttackObject::MonsterAttackObject() :
		m_pOwner(nullptr)
	{
		
		AddComponent<MonsterAttackScript>();
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetActive(false);
	}

	MonsterAttackObject::~MonsterAttackObject()
	{

	}

	void MonsterAttackObject::Initialize()
	{

	}

	void MonsterAttackObject::Update()
	{
		Vector3 vPosition = m_pOwner->GetComponent<Transform>()->GetPosition();
		GetComponent<Transform>()->SetPosition(vPosition);

		GameObject::Update();
	}

	void MonsterAttackObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	//void MonsterAttackObject::Render()
	//{
	//	GameObject::Render();
	//}

	void MonsterAttackObject::SetMonsterAttack(const tMonsterAttack& _tAttackInfo)
	{
		//데미지
		GetScript<MonsterAttackScript>()->SetAttackInfo(_tAttackInfo.tAttackInfo);
		Transform* pTransform = GetComponent<Transform>();
		
		pTransform->SetPosition(_tAttackInfo.vPosition);
		pTransform->SetRotation(_tAttackInfo.vRoatate);

		//오프셋
		int iDir = m_pOwner->GetDir();
		Collider2D* pCollider = GetComponent< Collider2D>();
		pCollider->SetCenter(Vector2(_tAttackInfo.vOffset.x * iDir, _tAttackInfo.vOffset.y));
		pCollider->SetSize(_tAttackInfo.vScale);

		GetComponent<Collider2D>()->SetActive(true);
	}

}