#include "WShuriken.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WAttackScript.h"
#include "WSceneManger.h"
#include "WTime.h"
#include "WEventManager.h"
namespace W
{
	UINT Shuriken::CREATE_ID = 0;
	UINT Shuriken::COUNT = 0;

	Shuriken::Shuriken():
		m_iDir(1)
	{
		m_iCount = COUNT;
		++COUNT;

	
		Animator* pAnimator = AddComponent<Animator>();
		
		pAnimator->Create(L"shuriken_left", Vector2(0.0f, 0.0f), Vector2(48.0f, 9.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"shuriken_right", Vector2(48.0f, 0.0f), Vector2(-48.0f, 9.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.1f);
	}

	Shuriken::~Shuriken()
	{

	}

	void Shuriken::Initialize()
	{
		AddComponent<AttackScript>();

		Collider2D* pCollider = AddComponent<Collider2D>();
		GetComponent<Transform>()->SetScale(1.f, 1.f, 0.f);
		pCollider->SetSize(Vector2(0.5f, 0.5f));

	}

	//이동, 삭제시간, attackscript말고 각각의 클래스에서
	void Shuriken::Update()
	{	
		GameObject::Update();
	}

	void Shuriken::LateUpdate()
	{
		Transform* pTr =GetComponent<Transform>();
		Vector3 vPosition = pTr->GetPosition();
		vPosition.x += (m_iDir * 8.f * Time::DeltaTime());
		pTr->SetPosition(vPosition);

		GameObject::LateUpdate();
	}

	void Shuriken::SetDir(int _iDir)
	{
		m_iDir = _iDir;
		AttackScript* pScript = GetScript<AttackScript>();
		pScript->SetDir(m_iDir);

		Animator* pAnimator = GetComponent<Animator>();
		std::wstring strDir;
		if (m_iDir > 0)
			strDir = L"_right";
		else
			strDir = L"_left";
		pAnimator->Play(L"shuriken" + strDir, true);
	}
	void Shuriken::Off()
	{
		PlayerAttackObject::Off();
	}
}