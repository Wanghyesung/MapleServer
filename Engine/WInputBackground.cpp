#include "WInputBackground.h"
#include "WResources.h"
#include "WGroggy.h"
#include "WInputObject.h"
#include "WSceneManger.h"
#include "WEventManager.h"
#include "WSceneManger.h"
namespace W
{
	UINT InputBackground::CREATE_ID = 0;
	InputBackground::InputBackground():
		m_pOwner(nullptr),
		m_vecInput{},
		m_iCurIndex(0)
	{
		
		GetComponent<Transform>()->SetScale(2.5f * 1.8f, 2.5f * 0.5f, 0.f);
	}
	InputBackground::~InputBackground()
	{
	
	}
	void InputBackground::Initialize()
	{
		Vector3 vTargetPos = Vector3(0.f, 0.f, -10.f);
		vTargetPos.y += 1.f;
		vTargetPos.z += 1.f;

		UINT iIndex = 0;
		GetComponent<Transform>()->SetPosition(vTargetPos);
	
		for (int i = -2; i < 3; ++i)
		{
			UINT iStart = (UINT)eKeyCode::UP;
			UINT iEnd = (UINT)eKeyCode::RIGHT;

			std::random_device rDiv;
			std::mt19937 en(rDiv());
			std::uniform_int_distribution<int> num(iStart, iEnd);
			int iNum = (int)num(en);
			
			wstring strDir = InputObject::GetWDir((eKeyCode)iNum);
			GameObject* pObj = ObjectPoolManager::PopObject(L"Input" + strDir);
			InputObject* pInputObj = static_cast<InputObject*>(pObj);

			pObj->Initialize();
			pInputObj->SetOwner(this);
			pInputObj->SetSceneID(GetSceneID());
			pInputObj->SetTarget(m_pTarget);
			pInputObj->SetIndex(iIndex);

			float x = vTargetPos.x + (i * 0.8f);
			Vector3 vPos = Vector3(x, vTargetPos.y, vTargetPos.z - 0.2f);
			pInputObj->GetComponent<Transform>()->SetPosition(vPos);

			m_vecInput.push_back(pInputObj);
			EventManager::CreateObject(pInputObj, eLayerType::Object);

			++iIndex;
		}
	}
	void InputBackground::Update()
	{
		if(m_bNext)
			next_button();
	
		GameObject::Update();
	}
	void InputBackground::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void InputBackground::UpdatePacket()
	{
		
	}
	

	void InputBackground::Failed()
	{
		EventManager::DeleteObject(this);
		static_cast<Groggy*>(m_pOwner)->SetTime(12.f);

		for (int i = 0; i < m_vecInput.size(); ++i)
			EventManager::AddMonsterPool(m_vecInput[i]);
	}

	void InputBackground::Next()
	{
		m_bNext = true;
	}

	void InputBackground::SetTarget(Player* _pPlayer)
	{
		ClearExclusiveClients();
		m_pTarget = _pPlayer;
		AddExclusiveClient(_pPlayer->GetPlayerID());
	}

	void InputBackground::next_button()
	{
		++m_iCurIndex;
		if (m_iCurIndex >= m_vecInput.size())
		{
			static_cast<Groggy*>(m_pOwner)->SetTime(0.f);
			EventManager::DeleteObject(this);

			for (int i = 0; i < m_vecInput.size(); ++i)
				EventManager::AddMonsterPool(m_vecInput[i]);
		}

		m_bNext = false;
	}
}