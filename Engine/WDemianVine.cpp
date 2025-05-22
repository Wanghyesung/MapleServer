#include "WDemianVine.h"
#include "WEffect.h"
#include "WResources.h"
#include "WAnimator.h"
#include "WLight.h"
#include "WTime.h"
#include "WMonster.h"
#include "WSceneManger.h"

#include "WWhite.h"
#include "WEventManager.h"
#include "WPlayerScript.h"
#include "WMonsterScript.h"


#define CLEAR_VALUE 10
namespace W
{
	UINT DemianVine::CREATE_ID = 0;

	DemianVine::DemianVine() :
		m_fDeleteTime(6.f),
		m_pWhite(nullptr),
		m_bActive(false)
	{

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"vine", Vector2(0.0f, 0.0f), Vector2(244.f, 150.f), 9, Vector2(250.f, 250.f), Vector2::Zero, 0.15f);


	
		GetComponent<Transform>()->SetScale(18.f,18.f,0.f);

		m_pWhite = new White();
		m_pWhite->SetEndTime(2.f);
		Vector3 vPos = m_pWhite->GetComponent<Transform>()->GetPosition();
		vPos.z = -3.1f;
		m_pWhite->GetComponent<Transform>()->SetPosition(vPos);

		m_pWhite->SetFunction(std::bind(&DemianVine::attack, this));


		m_tAttackInfo.fAttackDamage = BattleManager::GetMaxDamage();
		m_tAttackInfo.fAttRcnt = 0.f;//맞으
		m_tAttackInfo.fAttUpperRcnt = 0.f;
	}
	DemianVine::~DemianVine()
	{
		if (!m_bActive)
		{
			delete m_pWhite;
			m_pWhite = nullptr;
		}
	}
	void DemianVine::Initialize()
	{
		const tObjectInfo& tInfo = GetOwner()->GetScript<MonsterScript>()->GetObjectInfo();
		m_fDemianHP = tInfo.fHP;

		Animator* pAnim = GetComponent<Animator>();
		pAnim->Play(L"vine", true);
	}
	void DemianVine::Update()
	{
		if (m_bStart)
		{
			m_fDeleteTime -= Time::DeltaTime();
			if (m_fDeleteTime <= 0.f)
			{
				m_bStart = false;
				fadein();
			}
		}
		else if(!m_bActive && !m_bStart)
		{
			bool bEnd = GetComponent<Animator>()->GetActiveAnimation()->IsComplete();
			if (bEnd)
			{
				GetComponent<Animator>()->Stop(true);
				start();
			}
				
		}

		if(m_bActive)
			check_hp();

		GameObject::Update();
	}

	void DemianVine::LateUpdate()
	{
		GameObject::LateUpdate();
	}


	void DemianVine::start()
	{
		m_bStart = true;
		GetComponent<Animator>()->Stop(true);
	}

	void DemianVine::fadein()
	{
		m_bActive = true;
		m_pWhite->Initialize();
		EventManager::CreateObject(m_pWhite,eLayerType::Object);
	}

	void DemianVine::off()
	{
		m_fDeleteTime = 6.f;
		m_bStart = false;
		m_bActive = false;
		m_bClear = false;

		GetComponent<Animator>()->Stop(false);
		GetComponent<Animator>()->FindAnimation(L"vine")->SetIndex(0);
		EventManager::AddMonsterPool(this);
	}

	void DemianVine::check_hp()
	{
		const tObjectInfo& tInfo = GetOwner()->GetScript<MonsterScript>()->GetObjectInfo();

		//일정 데미지를 넣으면 해제
		if (CLEAR_VALUE <= m_fDemianHP - tInfo.fHP)
		{
			m_bClear = true;
		}
	}

	void DemianVine::attack()
	{
		for (GameObject* pPlayer : SceneManger::GetPlayers(GetSceneName()))
		{
			if (!m_bClear)
				pPlayer->GetScript<PlayerScript>()->Hit(m_tAttackInfo, L"DemianVine");
		}

		off();
	}
}