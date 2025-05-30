#pragma once

#include "WScene.h"


namespace W
{
	class Player;
	class SceneManger
	{
	public:
		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void UpdatePacket();

		static void Release();
		static void Erase(GameObject* _pGameObject);

		template <typename T>
		static bool CreateScene(const std::wstring& name)
		{
			T* scene = new T();

			std::unordered_map<std::wstring, Scene*>::iterator iter
				= m_hashScene.find(name);

			if (iter != m_hashScene.end())
				return false;

			m_hashScene.insert(std::make_pair(name, scene));

			scene->SetSceneIdx(SCENE_IDX++);
			scene->SetName(name);
			scene->Initialize();

			return true;
		}

		static Scene* FindScene(const std::wstring& _strSceneName);
		static Scene* GetActiveScene(GameObject* _pGameObj);
		static std::vector<Scene*> GetPlayerScene();
		
		//여기서 서버가 클라에게 패킷 전달
		static void AddGameObject(const std::wstring& _strSceneName, eLayerType _eType, GameObject* _pGameObj);
		static GameObject* FindPlayer(UINT _iPlayerID);
		static GameObject* FindPlayer(const std::wstring& strSceneName);
		static std::vector<GameObject*> GetPlayers(const std::wstring& _strSceneName);
		static std::vector<UINT> GetPlayerIDs(const std::wstring& _strSceneName);

		static void SwapObject(Scene* _pPrevScene, Scene* _pNextScene, GameObject* _pGameObject);
		static void SwapPlayer(Player* _pPlayer, Scene* _pPrevScene, Scene* _pNextScene);
		static void PushObjectPool(Scene* _pPrevScene);
		static void AddPlayerScene(Player* pPlayer, const std::wstring& _strScene);

	private:
		static UINT SCENE_IDX;
		static std::unordered_map<std::wstring, Scene*> m_hashScene;
		static std::unordered_map<std::wstring, std::vector<UINT>> m_hashPlayerScene;//플레이어가 있는 구간만 업데이트
	};
}


