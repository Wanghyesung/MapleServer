#include "WBlast.h"
#include "WAttackScript.h"
#include "WAnimator.h"
#include "WEventManager.h"
namespace W
{
	Blast::Blast()
	{
		Animator* pAnim = AddComponent<Animator>();
		
		pAnim->Create(L"left", Vector2(0.0f, 0.0f), Vector2(695.0f, 506.0f), 23, Vector2(700.f, 700.f), Vector2::Zero, 0.06f);
		pAnim->Create(L"right", Vector2(15290.0f, 0.0f), Vector2(-695.0f, 506.0f), 23, Vector2(700.f, 700.f), Vector2::Zero, 0.06f);

		pAnim->CompleteEvent(L"right") = std::bind(&EventManager::AddPlayerPool, this);
		pAnim->CompleteEvent(L"left") = std::bind(&EventManager::AddPlayerPool, this);

	}
	Blast::~Blast()
	{

	}
	void Blast::Initialize()
	{
		AttackScript* pScript = AddComponent<AttackScript>();
		pScript->SetDir(m_iDir);
		pScript->SetStayObject(5, 0.5f);
		pScript->SetDeleteTime(20.f);

		GetComponent<Transform>()->SetScale(8.f, 8.f, 0.f);
		Collider2D* pCollider = AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.4f, 0.4f));
	}
	void Blast::Update()
	{
		GameObject::Update();
	}
	void Blast::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Blast::Off()
	{
		PlayerAttackObject::Off();
	}
	void Blast::SetDir(int _iDir)
	{
		Animator* pAnim = GetComponent<Animator>();
		m_iDir = _iDir;
		if (m_iDir > 0)
			pAnim->Play(L"right", true);
		else
			pAnim->Play(L"left", true);

	}
	
}