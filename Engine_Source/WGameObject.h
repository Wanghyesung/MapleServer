#pragma once

#include "WEntity.h"
#include "WComponent.h"
#include "WScript.h"

namespace W
{
	class GameObject : public Entity
	{
	public:
		enum eState
		{
			Active,
			Paused,
			Dead,
		};

		GameObject();
		GameObject(const GameObject& _pOrigin);
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void UpdatePacket();

		template <typename T>
		T* GetComponent()
		{
			T* component;
			for (Component* comp : m_vecComponent)
			{
				component = dynamic_cast<T*>(comp);
				if (component != nullptr)
					return component;
			}

			for (Component* script : m_vecScript)
			{
				component = dynamic_cast<T*>(script);
				if (component != nullptr)
					return component;
			}


			return nullptr;
		}

		template<typename T>
		T* AddComponent()
		{
			T* comp = new T();
			
			Component* buff =
				dynamic_cast<Component*>(comp);

			Script* script =
				dynamic_cast<Script*>(buff);

			if (buff == nullptr)
				return nullptr;

			if (script == nullptr)
				m_vecComponent.push_back(buff);
			else
				m_vecScript.push_back(script);

			comp->SetOwner(this);

			return comp;
		}

		template<typename T>
		std::vector<T*> GetComponents()
		{
			std::vector<T*> comps;

			T* component;
			for (Component* comp : m_vecComponent)
			{
				component = dynamic_cast<T*>(comp);
				if (component != nullptr)
					comps.push_back(component);
			}

			for (Script* script : m_vecScript)
			{
				component = dynamic_cast<T*>(script);
				if (component != nullptr)
					comps.push_back(component);
			}

			return comps;
		}

		template<typename T>
		T* GetScript()
		{
			for (Script* pScript : m_vecScript)
			{
				T* pTarget = dynamic_cast<T*>(pScript);
				if (pTarget != nullptr)
					return pTarget;
			}
		}
		
		void SetState(eState _eState) { m_eState = _eState; }
		eState GetState() { return m_eState; }

		void SetLayerType(eLayerType _eLayerType) { m_eLayerType = _eLayerType; }
		eLayerType GetLayerType() { return m_eLayerType; }

		void SetSceneName(const std::wstring& _strName) { m_strSceneName = _strName; }
		const std::wstring& GetSceneName() { return m_strSceneName; }
		
		bool IsPoolObject() { return m_bPoolObject; }
		void SetPoolObject(bool _bPoolObject) { m_bPoolObject = _bPoolObject; }

		bool IsRender() { return m_bRender; }
		void SetRender(bool _bRender) { m_bRender = _bRender; }

		void SetObjectID(UINT _ID) { m_iObjectID = _ID; }
		UINT GetObjectID() { return m_iObjectID; }

		DECLARE_CREATE_ID;

	private:
		eState m_eState;
		std::vector<Component*> m_vecComponent;
		std::vector<Script*> m_vecScript;
		eLayerType m_eLayerType;
		std::wstring m_strSceneName;

		bool m_bRender;
		bool m_bPoolObject;

		UINT m_iObjectID;

		friend class GameObjectManager;
	};
}


