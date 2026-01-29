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
		virtual void UpdatePacket();

		template<typename T>
		T* FindObject();

		virtual void DestroyAll(Scene* _pScene);

		GameObject* FindObject(UINT _iObjectID);
		void AddGameObject(GameObject* _pGameObj);
		const unordered_map<UINT, GameObject*>& GetGameObjects();
		std::unordered_map<UINT, GameObject*> GetCopyGameObjects();

		void EraseOnHash(GameObject* _pGameObject);

		UINT GetCurObjectID();
		void PushObjectID(UINT _iID);
	private:
	
		unordered_map<UINT, GameObject*> m_hashGameObject;

		queue<UINT> m_queueObjectID;
	

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


