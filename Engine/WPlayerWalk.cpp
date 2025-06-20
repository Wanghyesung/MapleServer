#include "WPlayerWalk.h"
#include "WRigidbody.h"
#include "WInput.h"
#include "WPlayerScript.h"
#include "WEventManager.h"

namespace W
{
	PlayerWalk::PlayerWalk()
	{

		SetType(Player::ePlayerState::walk);
		SetStateName(L"_walk");
	}
	PlayerWalk::~PlayerWalk()
	{

	}
	void PlayerWalk::Update()
	{
		Player* pPlayer = GetPlayer();
		tObjectInfo tInfo = pPlayer->GetScript<PlayerScript>()->GetObjectInfo();
		if (Input::GetKey(pPlayer->GetPlayerID(), eKeyCode::X))
		{
			bool bStab = GetPlayer()->GetScript<PlayerScript>()->IsStab();
			if (bStab)
			{
				EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::stab);
				return;
			}

			EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::swing1);
			return;
		}

		Rigidbody* pRigidbody = GetPlayer()->GetComponent<Rigidbody>();
		if (Input::GetKey(pPlayer->GetPlayerID(), eKeyCode::RIGHT))
		{
			pRigidbody->AddForce(Vector2(tInfo.fSpeed, 0.f));
			GetPlayer()->SetDir(1);
		}
		else if (Input::GetKeyUp(pPlayer->GetPlayerID(), eKeyCode::RIGHT))
		{
			EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::stand);
		}

		if (Input::GetKey(pPlayer->GetPlayerID(), eKeyCode::LEFT))
		{
			pRigidbody->AddForce(Vector2(-tInfo.fSpeed, 0.f));
			GetPlayer()->SetDir(-1);
		}

		else if (Input::GetKeyUp(pPlayer->GetPlayerID(), eKeyCode::LEFT))
		{
			EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::stand);
		}

		if (Input::GetKeyDown(pPlayer->GetPlayerID(), eKeyCode::SPACE))
		{
			Rigidbody* pRigidbody = GetPlayer()->GetComponent<Rigidbody>();
			if (!pRigidbody->IsGround())
				return;
			EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::jump);

			int iDir = GetPlayer()->GetDir();
			pRigidbody->SetVelocity(Vector2(iDir * 2.f, 6.f));
			pRigidbody->SetGround(false);
		}

	}
	void PlayerWalk::Enter()
	{

	}
	void PlayerWalk::Exit()
	{

	}
}