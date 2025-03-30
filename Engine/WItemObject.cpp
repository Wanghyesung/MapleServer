#include "WItemObject.h"
#include "WRigidbody.h"
#include "WResources.h"
#include "WTime.h"
#include "WItemScript.h"
#include "WPlayer.h"
#include "WSceneManger.h"

#include "WObject.h"
namespace W
{
	ItemObject::ItemObject(const std::wstring _strItemName):
		m_bActive(false),
		m_strItemName(_strItemName)
	{
		GetComponent<Transform>()->SetScale(0.1f * 30/10.f, 0.1f * 30/10.f, 0.f);
		
		AddComponent<Rigidbody>();
		AddComponent<ItemScript>();
		AddComponent<Collider2D>();
	}

	ItemObject::~ItemObject()
	{

	}

	void ItemObject::Initialize()
	{

	}

	void ItemObject::Update()
	{
		if (m_bActive)
			move_player();
		else
		{
			Vector3 vRotation = GetComponent<Transform>()->GetRotation();

			if (!GetComponent<Rigidbody>()->IsGround())
				vRotation.z += XM_PI * Time::DeltaTime() * 4;
			else
				vRotation.z = 0.f;

			GetComponent<Transform>()->SetRotation(vRotation);
		}

		GameObject::Update();
	}

	void ItemObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	//void ItemObject::Render()
	//{
	//	GameObject::Render();
	//}

	void ItemObject::SetVelociy(const Vector2& _vVel)
	{
		GetComponent<Rigidbody>()->SetGround(false);
		GetComponent<Rigidbody>()->SetVelocity(_vVel);
	}
	void ItemObject::move_player()
	{
		GameObject* pPlayer = SceneManger::FindPlayer();
		Vector3 vTargetPos = pPlayer->GetComponent<Transform>()->GetPosition();
		Vector3 vPos = GetComponent<Transform>()->GetPosition();
		vTargetPos.z = 0.f;
		vPos.z = 0.f;

		Vector3 vDiff = vTargetPos - vPos;
		float fLen = vDiff.Length();
		if (fLen <= 0.1f)
		{
			add_item();
			return;
		}

		vDiff.Normalize();
		vDiff *= 2.f;
		GetComponent<Rigidbody>()->AddForce(Vector2(vDiff.x, vDiff.y));
	}
	void ItemObject::add_item()
	{
		/*IconUI* pIcon = ItemManager::GetClone(m_strItemName);
		assert(pIcon);
		SceneManger::GetUI<Inventory>()->AddItem(pIcon, pIcon->GetName());
		object::Destroy(this);*/
	}
}