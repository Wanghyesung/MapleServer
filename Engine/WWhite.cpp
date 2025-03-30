#include "WWhite.h"
#include "WResources.h"

#include "WCameraScript.h"
#include "WSceneManger.h"
#include "WTime.h"
namespace W
{
	White::White():
		m_vColor(Vector4::Zero)
	{
		
		GetComponent<Transform>()->SetScale(12.f * 1.52f, 22.f * 1.f, 0.f);
		GetComponent<Transform>()->SetPosition(0.f,0.f,-3.f);
	}

	White::~White()
	{

	}

	void White::Initialize()
	{
		m_vColor = Vector4::One;
		m_vColor.w = 0.f;
	}

	void White::Update()
	{
		if (m_vColor.w >= 1.f)
		{
			m_vColor = Vector4::Zero;
			SceneManger::Erase(this);
			m_pCompleteEvent();
			return;
		}
		m_vColor.w += (Time::DeltaTime() / m_fEndTime);

		GameObject::Update();
	}

	void White::LateUpdate()
	{
		GameObject::LateUpdate();
	}


	
}