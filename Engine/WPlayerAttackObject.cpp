#include "WPlayerAttackObject.h"
#include "WAttackScript.h"
#include "WEffect.h"
#include "WSceneManger.h"
#include "WPlayer.h"
#include "WPlayerScript.h"
#include "WEventManager.h"
namespace W
{
	UINT PlayerAttackObject::CREATE_ID = 0;

	PlayerAttackObject::PlayerAttackObject():
		m_pPlayer(nullptr)
	{

	}
	PlayerAttackObject::~PlayerAttackObject()
	{
		
	}
	void PlayerAttackObject::Initialize()
	{
		AddComponent<AttackScript>()->SetDeleteTime(0.1f);

		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.4f, 0.2f));
	}
	void PlayerAttackObject::Update()
	{
		GameObject::Update();
	}
	void PlayerAttackObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void PlayerAttackObject::Off()
	{
		GetScript<AttackScript>()->off();

		GetComponent<Collider2D>()->SetActive(false);
	}
	void PlayerAttackObject::PushObjectPool()
	{
		EventManager::AddPlayerPool(this);
	}
	
}