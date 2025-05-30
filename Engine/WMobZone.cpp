#include "WMobZone.h"
#include "WResources.h"
#include "WPlayer.h"
#include "WSceneManger.h"
#include "WMonster.h"
#include "WBattleManager.h"
#include "WPlayerScript.h"
#include "WAnimator.h"
#include "WInfo.h"
#include "WTime.h"
namespace W
{
	UINT MobZone::CREATE_ID = 0;

	MobZone::MobZone():
		m_pTarget(nullptr),
		m_pMonster(nullptr),
		m_iCurLevel(0),
		m_tAttackInfo{},
		m_fAttackTime(2.f),
		m_fCurTime(0.f)
	{
		
		GetComponent<Transform>()->SetScale(10.f, 10.f, 0.f);

		Animator* pAnim = AddComponent<Animator>();
		pAnim->Create(L"Megnus_Zone0", Vector2(0.0f, 0.0f), Vector2(796.f, 852.0f), 8, Vector2(1100.f, 1100.f), Vector2::Zero, 0.07f);
		
		pAnim->Play(L"Megnus_Zone0", true);
	}
	MobZone::~MobZone()
	{

	}
	void MobZone::Initialize()
	{
		m_tAttackInfo.fAttackDamage = 10.f;
		m_tAttackInfo.fAttRcnt = 0.f;//¸ÂÀ¸
		m_tAttackInfo.fAttUpperRcnt = 0.f;

		m_vecLength.push_back(3.3f);
		m_vecLength.push_back(2.7f);
		m_vecLength.push_back(2.1f);
		m_vecLength.push_back(1.5f);

	}
	void MobZone::Update()
	{
		

		Vector3 vPosition = m_pMonster->GetComponent<Transform>()->GetPosition();
		GetComponent<Transform>()->SetPosition(vPosition);

		m_fCurTime += Time::DeltaTime();
		if (m_fCurTime >= m_fAttackTime)
		{
			m_fCurTime = 0.f;
			attack();
		}
		
		GameObject::Update();
	}
	void MobZone::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	
	void MobZone::SetLevel(UINT _iNum)
	{
		m_iCurLevel = _iNum;

		if (m_iCurLevel >= 4)
		{
			SetState(GameObject::eState::Paused);
			return;
		}

		std::wstring strLevel =  std::to_wstring(m_iCurLevel);
		
		Animator* pAnim = GetComponent<Animator>();
		switch (m_iCurLevel)
		{
		case 1:
			pAnim->Create(L"Megnus_Zone" + strLevel, Vector2(0.0f, 0.0f), Vector2(696.f, 852.0f), 8, Vector2(1100.f, 1100.f), Vector2::Zero, 0.15f);
			break;
		case 2:
			pAnim->Create(L"Megnus_Zone" + strLevel, Vector2(0.0f, 0.0f), Vector2(596.f, 852.0f), 8, Vector2(1100.f, 1100.f), Vector2::Zero, 0.15f);
			break;
		case 3:
			pAnim->Create(L"Megnus_Zone" + strLevel, Vector2(0.0f, 0.0f), Vector2(396.f, 852.0f), 8, Vector2(1100.f, 1100.f), Vector2::Zero, 0.15f);
			break;
		}
		
		pAnim->Play(L"Megnus_Zone" + strLevel, true);
	}
	void MobZone::attack()
	{
		for (GameObject* pObj : SceneManger::GetPlayers(GetSceneName()))
		{
			Player* pPlayer = static_cast<Player*>(pObj);
			if (pPlayer &&
				pPlayer->GetState() == GameObject::eState::Active)
			{
				float fPlayerX = pPlayer->GetComponent<Transform>()->GetPosition().x;
				float fX = GetComponent<Transform>()->GetPosition().x;

				float fLen = fPlayerX - fX;
				if (fabs(fLen) >= m_vecLength[m_iCurLevel])
					pPlayer->GetScript<PlayerScript>()->Hit(m_tAttackInfo, L"MobZone");
			}
		}
		

	}
}