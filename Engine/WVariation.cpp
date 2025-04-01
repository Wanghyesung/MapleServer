#include "WVariation.h"
#include "WResources.h"
#include "WAnimator.h"

#include "WRigidbody.h"
#include "WPlayer.h"
#include "WEventManager.h"
namespace W
{
	Variation::Variation()
	{
		
		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"variation", Vector2(0.0f, 0.0f), Vector2(128.0f, 123.0f), 7, Vector2(130.f, 130.f), Vector2::Zero, 0.12f);
		pAnim->CompleteEvent(L"variation") = std::bind(&Variation::off, this);
		pAnim->Play(L"variation", true);

		GetComponent<Transform>()->SetScale(Vector3(1.7f, 1.7f, 0.f));
	}
	Variation::~Variation()
	{

	}
	void Variation::Initialize()
	{

	}
	void Variation::Update()
	{
		Abnormal::Update();
	}
	void Variation::LateUpdate()
	{
		Abnormal::LateUpdate();
	}
	

	void Variation::off()
	{
		EventManager::DeleteObject(this);
	}


}