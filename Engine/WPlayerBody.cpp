#include "WPlayerBody.h"
#include "WAnimator.h"
#include "WResources.h"
#include "WPlayer.h"
#include "WPlayerTop.h"
#include "WPlayerBottom.h"
#include "WPlayerShoes.h"
namespace W
{
	UINT PlayerBody::CREATE_ID = 0;

	PlayerBody::PlayerBody() :
		m_pPlayer(nullptr),
		m_strCurAnim{},
		m_pPlayerTop(nullptr),
		m_pPlayerBottom(nullptr),
		m_pPlayerShoes(nullptr)
	{
		
	}
	PlayerBody::~PlayerBody()
	{
		if (m_pPlayerTop)
		{
			delete m_pPlayerTop;
			m_pPlayerTop = nullptr;
		}
		if (m_pPlayerBottom)
		{
			delete m_pPlayerBottom;
			m_pPlayerBottom = nullptr;
		}
		if (m_pPlayerShoes)
		{
			delete m_pPlayerShoes;
			m_pPlayerShoes = nullptr;
		}
	}
	void PlayerBody::Initialize()
	{
		Animator* pAnimator = AddComponent<Animator>();

		pAnimator->Create(L"body_alert_left", Vector2(0.0f, 0.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_jump_left", Vector2(0.0f, 150.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_ladder_left", Vector2(0.0f, 300.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_pronestab_left", Vector2(0.0f, 450.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_proneattack_left", Vector2(150.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);

		pAnimator->Create(L"body_stab_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_stand_left", Vector2(0.0f, 750.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_swing1_left", Vector2(0.0f, 900.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_swing2_left", Vector2(0.0f, 1050.0f), Vector2(150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_swingOF_left", Vector2(0.0f, 1200.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_walk_left", Vector2(0.0f, 1350.0f), Vector2(150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_swingQS_left", Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"body_swingQS_left")->Create(L"body_swingQS_left",Vector2(0.0f, 600.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"body_swingQS_left")->Create(L"body_swingQS_left",Vector2(450.0f, 1200.0f), Vector2(150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);

		pAnimator->Create(L"body_alert_right", Vector2(450.0f, 0.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_jump_right", Vector2(450.0f, 150.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_ladder_right", Vector2(450.0f, 300.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_pronestab_right", Vector2(450.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_proneattack_right", Vector2(300.0f, 450.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);

		pAnimator->Create(L"body_stab_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_stand_right", Vector2(450.0f, 750.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_swing1_right", Vector2(450.0f, 900.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_swing2_right", Vector2(450.0f, 1050.0f), Vector2(-150.0f, 150.0f), 3, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_swingOF_right", Vector2(450.0f, 1200.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_walk_right", Vector2(450.0f, 1350.0f), Vector2(-150.0f, 150.0f), 4, Vector2(120.f, 120.f), Vector2::Zero, 0.1f);
		pAnimator->Create(L"body_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 2, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"body_swingQS_right")->Create(L"body_swingQS_right", Vector2(450.0f, 600.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);
		pAnimator->FindAnimation(L"body_swingQS_right")->Create(L"body_swingQS_right", Vector2(0, 1200.0f), Vector2(-150.0f, 150.0f), 1, Vector2(120.f, 120.f), Vector2::Zero, 0.07f);

		Vector3 vScale = m_pPlayer->GetComponent<Transform>()->GetScale();
		GetComponent<Transform>()->SetScale(vScale);

		m_pPlayerTop = new PlayerTop();
		m_pPlayerTop->SetPlayerBody(this);

		m_pPlayerBottom = new PlayerBottom();
		m_pPlayerBottom->SetPlayerBody(this);

		m_pPlayerShoes = new PlayerShoes();
		m_pPlayerShoes->SetPlayerBody(this);
	
	}
	void PlayerBody::Update()
	{
		GameObject::Update();

		m_pPlayerBottom->Update();
		m_pPlayerTop->Update();
		m_pPlayerShoes->Update();
	}
	void PlayerBody::LateUpdate()
	{
		Animator* pAnimator = GetComponent<Animator>();
		Vector3 vPlayerPos = m_pPlayer->GetComponent<Transform>()->GetPosition();
		vPlayerPos.z -= 0.01f;
		GetComponent<Transform>()->SetPosition(vPlayerPos);

		int iDir = m_pPlayer->GetDir();
		std::wstring strDir;
		std::wstring strState;
		strDir = iDir > 0 ? L"_right" : L"_left";
		
		strState = m_pPlayer->GetCurStateName();

		std::wstring strAnim = L"body" + strState + strDir;
		
		if (m_strCurAnim != strAnim)
		{
			m_strCurAnim = strAnim;
			bool bLoop = m_pPlayer->IsLoop();

			pAnimator->Play(strAnim, bLoop);
		}

		GameObject::LateUpdate();

		m_pPlayerBottom->LateUpdate();
		m_pPlayerTop->LateUpdate();
		m_pPlayerShoes->LateUpdate();
	}
	
	void PlayerBody::SetEquipTop(const std::wstring& _strEquipName)
	{
		m_pPlayerTop->SetPlayerEquip(_strEquipName);
	}
	void PlayerBody::SetEquipBottom(const std::wstring& _strEquipName)
	{
		m_pPlayerBottom->SetPlayerEquip(_strEquipName);
	}
	void PlayerBody::SetEquipShoes(const std::wstring& _strEquipName)
	{
		m_pPlayerShoes->SetPlayerEquip(_strEquipName);
	}
	void PlayerBody::SetAnimationIndex()
	{
		if(m_strCurAnim.size() != 0)
			GetComponent<Animator>()->GetActiveAnimation()->SetIndex(0);
		if(m_pPlayerBottom->GetCurAnimName().size() != 0)
			m_pPlayerBottom->GetComponent<Animator>()->GetActiveAnimation()->SetIndex(0);
		if (m_pPlayerTop->GetCurAnimName().size() != 0)
			m_pPlayerTop->GetComponent<Animator>()->GetActiveAnimation()->SetIndex(0);
		if (m_pPlayerShoes->GetCurAnimName().size() != 0)
			m_pPlayerShoes->GetComponent<Animator>()->GetActiveAnimation()->SetIndex(0);

	}
	void PlayerBody::SetStop(bool _bStop)
	{
		GetComponent<Animator>()->Stop(_bStop);
		m_pPlayerBottom->GetComponent<Animator>()->Stop(_bStop);
		m_pPlayerTop->GetComponent<Animator>()->Stop(_bStop);
		m_pPlayerShoes->GetComponent<Animator>()->Stop(_bStop);
	}
}