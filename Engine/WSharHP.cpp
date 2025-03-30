#include "WSharHP.h"
#include "WResources.h"
#include "WMonsterBackHP.h"
#include "WSceneManger.h"
#include "WObject.h"
namespace W
{
	
	SharHP::SharHP():
		MonsterHP(true),
		m_fHP(100.f)
	{
	
	}
	SharHP::~SharHP()
	{
	}
	void SharHP::Initialize()
	{
		//보스용으로 만들기
		m_pHPBack = new MonsterBackHP(true);
		m_pHPBack->SetHP(this);
		SceneManger::AddGameObject(eLayerType::UI, m_pHPBack);
	}
	void SharHP::Update()
	{
		if (!IsActive())
			return;

		if (m_fHP <= 0.f)
		{
			object::Destroy(this);
			object::Destroy(m_pHPBack);
			return;
		}

		GameObject::Update();
	}
	void SharHP::LateUpdate()
	{
		if (!IsActive())
			return;

		GameObject::LateUpdate();
	}
	
}