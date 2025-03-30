#include "WConfusion.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WPlayer.h"
#include "WRigidbody.h"
#include "WInput.h"
#include "WPlayerScript.h"
#include "WInfo.h"
namespace W
{
	Confusion::Confusion()
	{

		GetComponent<Transform>()->SetScale(Vector3(0.5f, 0.5f, 0.f));

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"confusion", Vector2(0.0f, 0.0f), Vector2(29.0f, 46.0f), 5, Vector2(40.f, 40.f), Vector2::Zero, 0.15f);

		pAnim->Play(L"confusion", true);

	}
	Confusion::~Confusion()
	{

	}
	void Confusion::Initialize()
	{

	}
	void Confusion::Update()
	{
		Rigidbody* pRigidbody = m_pTarget->GetComponent<Rigidbody>();
		const tObjectInfo& tInfo = m_pTarget->GetScript<PlayerScript>()->GetObjectInfo();

		int iDir = m_pTarget->GetDir();
		if (m_pTarget->GetCurPlayerState() == Player::ePlayerState::walk)
		{
			if (Input::GetKey(eKeyCode::RIGHT))
			{
				pRigidbody->AddForce(Vector2(-tInfo.fSpeed, 0.f));
				iDir = -1;
				
			}
			else if (Input::GetKey(eKeyCode::LEFT))
			{
				iDir = 1;
				pRigidbody->AddForce(Vector2(tInfo.fSpeed, 0.f));
			
			}
			if (Input::GetKeyDown(eKeyCode::SPACE))
			{
				pRigidbody->SetVelocity(Vector2(iDir * 1.5f, 3.5f));
			}
		}
		else if (m_pTarget->GetCurPlayerState() == Player::ePlayerState::jump)
		{
			Vector2 vForece = Vector2::Zero;
			if (Input::GetKey(eKeyCode::RIGHT))
			{
				iDir = -1;
				vForece.x -= (tInfo.fSpeed - 1.f);
			}
			else if (Input::GetKey(eKeyCode::LEFT))
			{
				iDir = 1;
				vForece.x += (tInfo.fSpeed - 1.f);
			}
			pRigidbody->AddForce(vForece);
		}

		m_pTarget->SetDir(iDir);
		
		Abnormal::Update();
	}
	void Confusion::LateUpdate()
	{
		Abnormal::LateUpdate();
	}
	
}