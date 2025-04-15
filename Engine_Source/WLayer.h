#pragma once

#include "WGameObject.h"


namespace W
{
#define LAYER_STARAT_IDX 20

	class Scene;
	class Layer
	{
	public:
		Layer();
		~Layer();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		//virtual void Render();

		template<typename T>
		T* FindObject();

		virtual void DestroyAll(Scene* _pScene);

		void AddGameObject(GameObject* _pGameObj);
		const unordered_map<UINT, GameObject*>& GetGameObjects() { return m_hashGameObject; }

		void EraseOnVector(GameObject* _pGameObject);

	private:
		//UM���� ����
		std::unordered_map<UINT, GameObject*> m_hashGameObject;

		UINT m_iObjectID;

		friend class UIManger;
	};

	template<typename T>
	inline T* Layer::FindObject()
	{
		auto iter = m_hashGameObject.begin();
		for (iter; iter != m_hashGameObject.end(); ++iter)
		{
			T* pTarget = dynamic_cast<T*>(iter->second);
			if (pTarget == nullptr)
				continue;

			return pTarget;
		}
		return nullptr;
	}
}


