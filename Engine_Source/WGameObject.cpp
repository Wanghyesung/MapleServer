#include "WGameObject.h"
#include "WTransform.h"

namespace W
{
	UINT GameObject::CREATE_ID = 0;

	GameObject::GameObject() :
		m_eState(eState::Active),
		m_iObjectID(0),
		m_bPoolObject(false),
		m_bRender(true)
	{
		m_vecComponent.resize((UINT)eComponentType::End);
		AddComponent<Transform>();
	}

	GameObject::GameObject(const GameObject& _pOrigin):
		Entity(_pOrigin),
		m_eState(_pOrigin.m_eState),
		m_eLayerType(_pOrigin.m_eLayerType)
	{
		m_vecComponent.resize((UINT)eComponentType::End);

		for (Component* pCom : _pOrigin.m_vecComponent)
		{
			if (pCom == nullptr)
				continue;

			Component* pComponent = pCom->CreateClone();
			if (!pComponent)
				continue;

			UINT iComponentID = (UINT)pComponent->GetComponentType();
			m_vecComponent[iComponentID] = pComponent;
			pComponent->SetOwner(this);
		}
	}
	
	GameObject::~GameObject()
	{
		for (Component* comp : m_vecComponent)
		{
			if (comp == nullptr)
				continue;

			delete comp;
		}

		for (Component* script : m_vecScript)
		{
			if (script == nullptr)
				continue;

			delete script;
		}
	}
	void GameObject::Initialize()
	{
		
	}
	void GameObject::Update()
	{
		//생성위치 기록

		for (Component* comp : m_vecComponent)
		{
			if(comp)
				comp->Update();
		}

		for (Component* script : m_vecScript)
		{
			script->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		for (Component* comp : m_vecComponent)
		{
			if(comp)
				comp->LateUpdate();
		}

		for (Component* script : m_vecScript)
		{
			script->LateUpdate();
		}
	}

	void GameObject::UpdatePacket()
	{

	}
	
	
	
}