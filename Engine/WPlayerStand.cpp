#include "WPlayerStand.h"
#include "WInput.h"
#include "WPlayer.h"
#include "WRigidbody.h"
#include "WPlayerScript.h"
#include "WEventManager.h"

namespace W
{

	PlayerStand::PlayerStand()
	{
		SetType(Player::ePlayerState::stand);
		SetStateName(L"_stand");
	}
	PlayerStand::~PlayerStand()
	{

	}
	void PlayerStand::Update()
	{
		Player* pPlayer = GetPlayer();
		bool bIsAlert = GetPlayer()->IsAlert();
		if (bIsAlert)
		{
			EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::alert);
			//GetFSM()->ChangeState(Player::ePlayerState::alert);
		}

		if (Input::GetKey(pPlayer->GetPlayerID(), eKeyCode::X))
		{
			bool bStab = GetPlayer()->GetScript<PlayerScript>()->IsStab();
			if (bStab)
			{
				EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::stab);
				//GetFSM()->ChangeState(Player::ePlayerState::stab);
				return;
			}
			EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::swing1);

			//GetFSM()->ChangeState(Player::ePlayerState::swing1);
			return;
		}

		if (Input::GetKey(pPlayer->GetPlayerID(), eKeyCode::RIGHT))
		{
			EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::walk);
			//GetFSM()->ChangeState(Player::ePlayerState::walk);
			GetPlayer()->SetDir(1);
		}


		if (Input::GetKey(pPlayer->GetPlayerID(), eKeyCode::LEFT))
		{
			EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::walk);
			//GetFSM()->ChangeState(Player::ePlayerState::walk);
			GetPlayer()->SetDir(-1);
		}


		if (Input::GetKey(pPlayer->GetPlayerID(), eKeyCode::DOWN))
		{
			bool bLadder = GetPlayer()->GetScript<PlayerScript>()->IsLadder();

			if(bLadder)
				EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::ladder);
			else
			{
				EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::pronestab);
				if (Input::GetKey(pPlayer->GetPlayerID(), eKeyCode::SPACE))
					EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::jump);
			}
			return;
				
		}
		if (Input::GetKey(pPlayer->GetPlayerID(), eKeyCode::UP))
		{
			bool bLadder = GetPlayer()->GetScript<PlayerScript>()->IsLadder();
			if (bLadder)
			{
				EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::ladder);
			}
		}

		if (Input::GetKeyDown(pPlayer->GetPlayerID(), eKeyCode::SPACE))
		{
			Rigidbody* pRigidbody = GetPlayer()->GetComponent<Rigidbody>();
			if (!pRigidbody->IsGround())
				return;

			EventManager::ChangePlayerFSMState(GetFSM(), Player::ePlayerState::jump);
			pRigidbody->SetVelocity(Vector2(0.f, 6.f));
			pRigidbody->SetGround(false);
		}

	}
	void PlayerStand::Enter()
	{

	}
	void PlayerStand::Exit()
	{
		
	}
}