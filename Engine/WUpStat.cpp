#include "WUpStat.h"
#include "WMonster.h"
#include "WTime.h"
#include "WMonsterScript.h"
#include "WBattleManager.h"
namespace W
{
	UINT UpStat::CREATE_ID = 0;

	UpStat::UpStat(float _fAccStat):
		m_fCurTime(0.f),
		m_fAccStat(_fAccStat)
	{

	}
	UpStat::~UpStat()
	{

	}
	void UpStat::Initialize()
	{

	}
	void UpStat::Update()
	{
		GameObject::Update();
	}
	void UpStat::LateUpdate()
	{
		m_fCurTime += Time::DeltaTime();

		if (m_fCurTime >= m_fDeleteTime ||
			m_pTarget->GetState() == GameObject::Paused)
		{
			EventManager::DeleteObject(this);
			BattleManager::Buff_Stat(m_pTarget, m_eType, -m_fAccStat);
			return;
		}

		Vector3 vPosition = m_pTarget->GetComponent<Collider2D>()->GetPosition();
		vPosition.y += 0.5f;
		vPosition.z -= 0.2f;

		GetComponent<Transform>()->SetPosition(vPosition);

		GameObject::LateUpdate();
	}

	void UpStat::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();
	}
	
}