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
		static void Erase(GameObject* _pGameObject, UINT _iPlayerID);

		template <typename T>
		static bool CreateScene(const std::wstring& name)
		{
			T* scene = new T();

			m_hashSceneID.insert(std::make_pair(scene->GetSceneID(), scene));

			scene->SetName(name);
			scene->Initialize();

			return true;
		}

		static Scene* FindScene(UINT _iSceneID);
		static Scene* GetActiveScene(GameObject* _pGameObj);
		static std::vector<Scene*> GetPlayerScene();
		
		//여기서 서버가 클라에게 패킷 전달
		static void AddGameObject(UINT _iSceneID, eLayerType _eType, GameObject* _pGameObj);

		static GameObject* FindPlayer(UINT _iPlayerID);
		static GameObject* FindPlayerRandom(UINT _iSceneID);
		static GameObject* FindPlayer(UINT _iSceneID, UINT _iPlayerID);
		static std::vector<GameObject*> GetPlayers(UINT _iSceneID);
		static std::vector<UINT> GetPlayerIDs(UINT _iSceneID);

		static void SwapObject(Scene* _pPrevScene, Scene* _pNextScene, GameObject* _pGameObject);
		static void SwapPlayer(GameObject* _pPlayer, UINT _iPrevSceneID, UINT _iNextSceneID);
		static void RetrieveAttackObject(UINT _iPlayerID, UINT _iPrevSceneID);
		static void AddPlayerScene(Player* pPlayer, UINT _iSceneID);

		static void SendEnterScene(UINT _iPlayerID, UINT _iSceneID);
	private:
		static UINT SCENE_IDX;
		static std::unordered_map<UINT, Scene*> m_hashSceneID;
		static std::unordered_map<UINT, std::vector<UINT>> m_hashPlayerScene;//플레이어가 있는 구간만 업데이트
	};
}


