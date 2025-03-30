#pragma once

#include "WScene.h"
#include "WUI.h"

namespace W
{
	class SceneManger
	{
	public:
		static void Initialize();
		static void Update();
		static void LateUpdate();
		//static void Render();
		static void Destroy();
		static void Release();
		static void Erase(GameObject* _pGameObject);

		template <typename T>
		static bool CreateScene(std::wstring name)
		{
			T* scene = new T();

			std::map<std::wstring, Scene*>::iterator iter
				= m_mapScene.find(name);

			if (iter != m_mapScene.end())
				return false;

			m_mapScene.insert(std::make_pair(name, scene));
			m_pActiveScene = scene;
			scene->Initialize();
			return true;
		}
		static Scene* GetActiveScene() { return m_pActiveScene; }
		static Scene* LoadScene(std::wstring _strName);

		//여기서 서버가 클라에게 패킷 전달
		static void AddGameObject(eLayerType _eType, GameObject* _pGameObj);
		static GameObject* FindPlayer();

		template <typename T>
		static T* GetUI()
		{
			std::vector<UI*> vecUI = m_pActiveScene->FindObjectsOfType<UI>();
			for (UI* pUI : vecUI)
			{
				std::queue<UI*> queue;

				queue.push(pUI);

				while (!queue.empty())
				{
					UI* pTarget = queue.front();
					queue.pop();

					T* pTargetUI = dynamic_cast<T*>(pTarget);
					if (pTargetUI != nullptr)
						return pTargetUI;
					else
					{
						std::vector<UI*> vecChildUI = pTarget->GetChildUI();
						for (UI* pChildUI : vecChildUI)
						{
							queue.push(pChildUI);
						}
					}
				}
			}
			return nullptr;
		}

		static void SwapObject(Scene* _pPrevScene, Scene* _pNextScene, GameObject* _pGameObject);
		static void SwapUI(Scene* _pPrevScene, Scene* _pNextScene);
		static void SwapPlayer(Scene* _pPrevScene, Scene* _pNextScene);
		static void SwapCamera();
		static void PushObjectPool(Scene* _pPrevScene);
		static void AddPlayerScene(const std::wstring& _strScene);

	private:
		
		static Scene* m_pActiveScene;
		static std::map<std::wstring, Scene*> m_mapScene;
		static std::vector<std::wstring> m_vecPlayerScene;//플레이어가 있는 구간만 업데이트
	};
}


