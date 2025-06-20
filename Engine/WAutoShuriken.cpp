#include "WAutoShuriken.h"
#include "WSceneManger.h"
#include "WLayer.h"
#include "WAttackScript.h"
#include "WTime.h"
#include "WAnimator.h"
#include "WMonster.h"
#include "WPlayer.h"
#include "WPlayerScript.h"
#include "WEventManager.h"
namespace W
{
	UINT AutoShuriken::CREATE_ID = 0;

	AutoShuriken::AutoShuriken():
		m_pTarget(nullptr),
		m_vVelocity(Vector2::Zero),
		m_vMaxVelocity(Vector2(3.f, 3.f)),
		m_fCurTime(0.f),
		m_fDeleteTime(7.f),
		m_fContFoce(4.f)
	{
		m_iDir = 1;
	}

	AutoShuriken::~AutoShuriken()
	{

	}

	void AutoShuriken::Initialize()
	{
		const tAttackInfo& tplayerAttack = GetPlayer()->GetScript<PlayerScript>()->GetAttackInfo();
		tAttackInfo AttackInfo;
		
		AttackInfo.fAttackDamage = 1.f;
		AttackInfo.fAttack = tplayerAttack.fAttack;
		AttackInfo.fDex = tplayerAttack.fDex;
		
		AttackInfo.fAttRcnt = 0.7f;
		AttackInfo.fAttRigidityTime = 0.3f;
		AttackInfo.fAttUpperRcnt = 0.f;
		AttackInfo.iDamageCount = 1;

		AttackScript* pScript = AddComponent<AttackScript>();
		pScript->SetDeleteTime(m_fDeleteTime);
		pScript->SetDir(1);
		pScript->SetAttackInfo(AttackInfo);

		Collider2D* pCollider = AddComponent<Collider2D>();
		GetComponent<Transform>()->SetScale(1.f, 1.f, 0.f);
		pCollider->SetSize(Vector2(0.5f, 0.5f));

		Animator* pAnimator = AddComponent<Animator>();
		
		pAnimator->Create(L"shuriken_left", Vector2(0.0f, 0.0f), Vector2(48.0f, 9.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.05f);
		pAnimator->Create(L"shuriken_right", Vector2(48.0f, 0.0f), Vector2(-48.0f, 9.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.05f);


	}

	void AutoShuriken::Update()
	{
		if (!m_pTarget || !m_pTarget->GetComponent<Collider2D>()->IsActive())
		{
			//타겟을 잡지 못하면 바로 오브젝트 풀
			if (!set_target())
				GetScript<AttackScript>()->SetDelete();
		}
		
		else
		{
			Collider2D* pTargetColl = m_pTarget->GetComponent<Collider2D>();

			if (m_pTarget->GetState() == GameObject::Active &&
				pTargetColl->IsActive())
				move_target();

			else if (m_pTarget->GetState() != GameObject::Active ||
				!pTargetColl->IsActive())
				set_target();
		}

		GameObject::Update();
	}
	void AutoShuriken::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void AutoShuriken::SetDir(const Vector2& _vDir)
	{
		m_vDir = _vDir;
	
		std::wstring strDir;
		if (m_iDir > 0)
			strDir = L"_right";
		else
			strDir = L"_left";
		GetComponent<Animator>()->Play(L"shuriken" + strDir, true);

		float fRadian = atan2f(m_vDir.y, m_vDir.x);
		GetComponent<Transform>()->SetRotation(0.f, 0.f, fRadian);
	}

	void AutoShuriken::Off()
	{
		m_vVelocity = Vector2::Zero;
		m_pTarget = nullptr;

		PlayerAttackObject::Off();
	}

	bool AutoShuriken::set_target()
	{	
		const std::unordered_map<UINT, GameObject*> hashMonster =
			SceneManger::GetActiveScene(this)->GetLayer(eLayerType::Monster)->GetGameObjects();

		Vector3 vPosistion = GetComponent<Transform>()->GetPosition();
		float fMaxLen = 10000.f;
	
		if (hashMonster.size() == 0)
			return false;
			
		auto iter = hashMonster.begin();
		for (iter; iter != hashMonster.end(); ++iter)
		{
			GameObject* pObj = iter->second;

			if (pObj->GetState() != GameObject::Active)
				continue;
			if (!pObj->GetComponent<Collider2D>()->IsActive())
				continue;

			Vector3 vTargetPos = pObj->GetComponent<Transform>()->GetPosition();
			Vector3 vDiff = vTargetPos - vPosistion;
			vTargetPos.z = 0.f;
			vDiff.z = 0.f;

			float fLen = vDiff.Length();
			if (fLen < fMaxLen)
			{
				fMaxLen = fLen;
				m_pTarget = dynamic_cast<Monster*>(pObj);
			}
		}

		if (!m_pTarget ||  !m_pTarget->GetComponent<Collider2D>()->IsActive())
			return false;

		return true;
	}

	void AutoShuriken::move_target()
	{
		move();

		rotate();
	}

	void AutoShuriken::move()
	{
		Vector2 vDiff = get_dir();
		
		float fCurDeleteTime = GetScript<AttackScript>()->GetCurDeleteTime();
		m_vForce = (vDiff * m_fContFoce * fCurDeleteTime);

		float fDot = m_vVelocity.Dot(vDiff);
		if (fDot < 0)
			m_vForce *= (m_fContFoce * 2.f);
		
		m_vVelocity += Time::DeltaTime() * m_vForce; 

		Vector3 vPosition = GetComponent<Collider2D>()->GetPosition();
		vPosition = vPosition + m_vVelocity * Time::DeltaTime();

		GetComponent<Transform>()->SetPosition(vPosition);
	}

	void AutoShuriken::rotate()
	{
		float fRadian = atan2f(m_vVelocity.y, m_vVelocity.x);
		GetComponent<Transform>()->SetRotation(0.f, 0.f, fRadian);
	}

	const Vector2& AutoShuriken::get_dir()
	{
		Collider2D* pTargetColl = m_pTarget->GetComponent<Collider2D>();
		Transform* pTr = m_pTarget->GetComponent<Transform>();

		Vector3 vTargetPos = pTargetColl->GetPosition();
		Vector2 vTargetSize = pTargetColl->GetSize();

		Vector2 vRightScale = pTr->Right() * (vTargetSize * pTr->GetScale().x * 0.5f);
		Vector2 vUpScale = pTr->Up() * (vTargetSize * pTr->GetScale().y * 0.5f);
		vTargetPos.x -= vRightScale.x;
		vTargetPos.y -= vUpScale.y;

		Vector3 vPosition = GetComponent<Collider2D>()->GetPosition();
		float z = vPosition.z;
		vTargetPos.z = 0.f;
		vPosition.z = 0.f;

		Vector3 vTemDiff = vTargetPos - vPosition;

		return Vector2(vTemDiff.x, vTemDiff.y);
	}

	

}