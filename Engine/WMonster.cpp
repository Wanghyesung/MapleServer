#include "WMonster.h"
#include "WSceneManger.h"
#include "WCollisionManager.h"
#include "WObjectPoolManager.h"
namespace W
{
	UINT Monster::CREATE_ID = 0;

	Monster::Monster():
		m_iDir(1),
		m_eMonsterState(eMonsterState::stand),
		m_strCurStateName(L"_stand"),
		m_vecItemNames{},
		m_iCreateCount(1)
		//m_mapAttackObjs{}
	{
		SetPoolObject(true);
	}
	Monster::~Monster()
	{
		
	}
	void Monster::Initialize()
	{
		
	}
	void Monster::Update()
	{
		if (m_bDead)
			return;

		GameObject::Update();
	}
	void Monster::LateUpdate()
	{
		if (m_bDead)
			return;

		GameObject::LateUpdate();
	}

	void Monster::UpdatePacket()
	{
		GetComponent<Transform>()->SendTransform();
	}

	void Monster::SetItem(const std::vector<std::wstring>& _vecItemNames)
	{
		for (int i = 0; i < _vecItemNames.size(); ++i)
			m_vecItemNames.push_back(_vecItemNames[i]);
	}

	void Monster::AddMonsterSkill(MonsterAttackObject* _pObj)
	{
		ObjectPoolManager::AddObjectPool(_pObj->GetName(), _pObj);
	}

	MonsterAttackObject* Monster::GetMonsterSkill(const std::wstring& _strName)
	{
		GameObject* pGameObj = ObjectPoolManager::PopObject(_strName);
		pGameObj->SetSceneName(GetSceneName());
		return dynamic_cast<MonsterAttackObject*>(pGameObj);
	}

	

}