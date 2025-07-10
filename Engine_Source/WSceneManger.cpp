#include "WSceneManger.h"

#include "WRigidbody.h"
#include "WAnimator.h"


#include "..\Engine\WPlayer.h"
#include "..\Engine\WBattleManager.h"

#include "..\Engine\WPlayerAttackObject.h"
#include "..\Engine\WObjectPoolManager.h"
#include "..\Engine\WMonsterManager.h"
#include "..\Engine\WPlayerScript.h"
#include "..\Engine\WEventManager.h"
namespace W
{
	UINT SceneManger::SCENE_IDX = 0;

	std::unordered_map<UINT, Scene*> SceneManger::m_hashSceneID = {};
	std::unordered_map<UINT, std::vector<UINT>> SceneManger::m_hashPlayerScene = {};

	void SceneManger::Initialize()
	{
		//최대 동접자 3/5만 미리 할당
		for (int i = 0; i < 3; ++i)
		{
			Player* pPlayer = new Player();
			pPlayer->SetName(L"Player");
			ObjectPoolManager::AddObjectPool(L"Player", pPlayer);
		}
	}
	void SceneManger::Update()
	{
		auto iter = m_hashPlayerScene.begin();
		for (iter; iter != m_hashPlayerScene.end(); ++iter)
		{
			if (iter->second.size() > 0)
			{
				m_hashSceneID.find(iter->first)->second->Update();
			}
		}
	}

	void SceneManger::LateUpdate()
	{
		auto iter = m_hashPlayerScene.begin();
		for (iter; iter != m_hashPlayerScene.end(); ++iter)
		{
			if (iter->second.size() > 0)
			{
				m_hashSceneID.find(iter->first)->second->LateUpdate();
			}
		}
	}

	void SceneManger::UpdatePacket()
	{
		auto iter = m_hashPlayerScene.begin();
		for (iter; iter != m_hashPlayerScene.end(); ++iter)
		{
			if (iter->second.size() > 0)
			{
				m_hashSceneID.find(iter->first)->second->UpdatePacket();
			}
		}
	}

	void SceneManger::Release()
	{
		for (auto &iter : m_hashSceneID)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		ObjectPoolManager::Release();
		BattleManager::Release();
	}

	void SceneManger::Erase(GameObject* _pGameObject)
	{
		eLayerType eLayer = _pGameObject->GetLayerType();
		UINT iSceneID = _pGameObject->GetSceneID();

		Scene* pScene = FindScene(iSceneID);
		pScene->EraseObject(eLayer, _pGameObject);

		Protocol::S_DELETE pkt;
		UINT iObjectID = _pGameObject->GetObjectID();

		pkt.set_pool_object(_pGameObject->IsPoolObject());
		pkt.set_scene_layer_deleteid( ((UCHAR)iSceneID << 24) | (((UCHAR)eLayer) << 16) | iObjectID);

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(iSceneID));
	}

	void SceneManger::Erase(GameObject* _pGameObject, UINT _iPlayerID)
	{
		eLayerType eLayer = _pGameObject->GetLayerType();
		UINT iSceneID = _pGameObject->GetSceneID();

		Scene* pScene = FindScene(iSceneID);
		pScene->EraseObject(eLayer, _pGameObject);

		Protocol::S_DELETE pkt;
		UINT iObjectID = _pGameObject->GetObjectID();

		pkt.set_pool_object(_pGameObject->IsPoolObject());
		pkt.set_scene_layer_deleteid(((UCHAR)iSceneID << 24) | (((UCHAR)eLayer) << 16) | iObjectID);

		//무조건 패킷을 전송해야하는 클라
		auto& vecClient = _pGameObject->GetExclusiveClients();
		unordered_set<UINT> setTarget;
		for (int i = 0; i < vecClient.size(); ++i)
			setTarget.insert(vecClient[i]);

		setTarget.insert(_iPlayerID);

		for (UINT ID : SceneManger::GetPlayerIDs(iSceneID))
			setTarget.insert(ID);

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, setTarget);
	}


	Scene* SceneManger::FindScene(UINT _iSceneID)
	{
		const auto& iter = m_hashSceneID.find(_iSceneID);
		if (iter == m_hashSceneID.end())
			return nullptr;

		return iter->second;
	}

	Scene* SceneManger::GetActiveScene(GameObject* _pGameObj)
	{
		UINT iSceneID = _pGameObj->GetSceneID();
		Scene* pScene = FindScene(iSceneID);

		if(!pScene)
			return nullptr;

		return pScene;
	}

	std::vector<Scene*> SceneManger::GetPlayerScene()
	{
		std::vector<Scene*> vecPlayerScene = {};

		auto iter = m_hashPlayerScene.begin();
		for (iter; iter != m_hashPlayerScene.end(); ++iter)
		{
			if (iter->second.size() > 0)
			{
				vecPlayerScene.push_back(m_hashSceneID.find(iter->first)->second);
			}
		}
		return vecPlayerScene;
	}

	void SceneManger::AddGameObject(UINT _iSceneID, eLayerType _eType, GameObject* _pGameObj)
	{
		Scene* pScene = FindScene(_iSceneID);
		//다음 프레임에 넣기, 후처리에 넣기
		pScene->AddGameObject(_eType, _pGameObj);
	}

	GameObject* SceneManger::FindPlayer(UINT _iPlayerID)
	{
		for (Scene* pScene : GetPlayerScene())
		{
			const std::unordered_map<UINT,GameObject*>& hashObjs
				= pScene->GetLayer(eLayerType::Player)->GetGameObjects();

			auto iter = hashObjs.find(_iPlayerID);
			if (iter != hashObjs.end())
				return iter->second;

		}

		return nullptr;
	}

	GameObject* SceneManger::FindPlayer(UINT _iSceneID, UINT _iPlayerID)
	{
		Scene* pScene = m_hashSceneID.find(_iSceneID)->second;

		const std::unordered_map<UINT, GameObject*>& hashObjs
			= pScene->GetLayer(eLayerType::Player)->GetGameObjects();

		auto iter = hashObjs.find(_iPlayerID);
		if (iter != hashObjs.end())
			return iter->second;

		return nullptr;
	}

	void SceneManger::DeletePlayer(UINT _iPlayerID)
	{
		GameObject* pPlayer = FindPlayer(_iPlayerID);
		if (pPlayer)
		{
			//player를 들고있는 오브젝트들이 안전하게 반환할 수 있게 다음 프레임에 삭제
			pPlayer->SetState(GameObject::eState::Dead);

			EventManager::EraseObject(pPlayer);
			UINT iSceneID = pPlayer->GetSceneID();
			UINT iPlayerID = pPlayer->GetObjectID();

			auto& vecIDs = m_hashPlayerScene[iSceneID];
			auto  iter = std::find(vecIDs.begin(), vecIDs.end(), iPlayerID);
			if (iter != vecIDs.end()) 
			{
				int idx = iter - vecIDs.begin();   
				int lastIdx = vecIDs.size() - 1;
				std::swap(vecIDs[idx], vecIDs[lastIdx]);
				vecIDs.pop_back();
			}
		}
	}

	GameObject* SceneManger::FindPlayerRandom(UINT _iSceneID)
	{
		Scene* pScene = m_hashSceneID.find(_iSceneID)->second;

		const std::unordered_map<UINT, GameObject*>& hashObjs
			= pScene->GetLayer(eLayerType::Player)->GetGameObjects();

		if(hashObjs.size() <= 0)
			return nullptr;

		std::random_device rDiv;
		static std::mt19937 gen(rDiv());
		std::uniform_int_distribution<int> rand(0, hashObjs.size());
		int seq = rand(gen);

		auto iter = hashObjs.begin();
		for (int i = 0; i < seq -1; ++i)
			++iter;

		return iter->second;
	}

	vector<GameObject*> SceneManger::FindAllPlayer(UINT _iSceneID)
	{
		vector<GameObject*> vecTarget = {};
		vecTarget.reserve(m_hashPlayerScene[_iSceneID].size());

		auto& iter = m_hashSceneID[_iSceneID]->GetLayer(eLayerType::Player)->GetGameObjects();
		for (auto& pPair : iter)
			vecTarget.push_back(pPair.second);
		
		return vecTarget;
	}


	std::vector<GameObject*> SceneManger::GetPlayers(UINT _iSceneID)
	{
		std::vector<GameObject*> vecPlayer;

		const std::unordered_map<UINT, GameObject*>& hashObjs
			= m_hashSceneID[_iSceneID]->GetLayer(eLayerType::Player)->GetGameObjects();

		auto iter = hashObjs.begin();
		for (iter; iter != hashObjs.end(); ++iter)
		{
			vecPlayer.push_back(iter->second);
		}
		return vecPlayer;
	}

	std::vector<UINT> SceneManger::GetPlayerIDs(UINT _iSceneID)
	{
		return m_hashPlayerScene[_iSceneID];
	}

	void SceneManger::SwapObject(Scene* _pPrevScene, Scene* _pNextScene, GameObject* _pGameObject)
	{
		eLayerType eType = _pGameObject->GetLayerType();
		
		_pPrevScene->EraseObject(eType, _pGameObject);
		_pNextScene->AddGameObject(eType, _pGameObject);
	}


	void SceneManger::SwapPlayer(GameObject* _pPlayer, UINT _iPrevSceneID, UINT _iNextSceneID)
	{
		Vector3 vPos = _pPlayer->GetComponent<Transform>()->GetPosition();
		vPos.x = 0.f; vPos.y = 0.f;
		_pPlayer->GetComponent<Transform>()->SetPosition(vPos);
		_pPlayer->GetComponent<Rigidbody>()->SetGround(false);
		
		Scene* pPrevScene = FindScene(_iPrevSceneID);
		Scene* pNextScene = FindScene(_iNextSceneID);
		pNextScene->AddGameObject(eLayerType::Player, _pPlayer);

		UINT iPlayerID = _pPlayer->GetObjectID(); //obj == playerid
		auto& iterPrev = m_hashPlayerScene[_iPrevSceneID];
		iterPrev.erase(
			std::remove(iterPrev.begin(), iterPrev.end(), iPlayerID),
			iterPrev.end()
		);

		// 다음 씬에 플레이어 ID 추가
		m_hashPlayerScene[_iNextSceneID].push_back(iPlayerID);

		pPrevScene->OnExitPlayer(iPlayerID);
		pNextScene->OnEnterPlayer(iPlayerID);
	}


	void SceneManger::RetrieveAttackObject(UINT _iPlayerID, UINT _iPrevSceneID)
	{
	 	Scene* pPrevScene = FindScene(_iPrevSceneID);
		const std::unordered_map<UINT, GameObject*>& hashObjs
			= pPrevScene->GetLayer(eLayerType::AttackObject)->GetGameObjects();

		auto iter = hashObjs.begin();
		for (iter; iter != hashObjs.end(); ++iter)
		{
			UINT iPlayerId = static_cast<PlayerAttackObject*>(iter->second)->GetPlayer()->GetPlayerID();
			if(iPlayerId == _iPlayerID)
				EventManager::AddPlayerPool(iter->second);
		}
			
	}

	void SceneManger::AddPlayerScene(Player* pPlayer, UINT _iSceneID)
	{
		AddGameObject(_iSceneID, eLayerType::Player, pPlayer);

		m_hashPlayerScene[_iSceneID].push_back(pPlayer->GetPlayerID());
	}

	void SceneManger::SendEnterScene(UINT _iPlayerID, UINT _iSceneID)
	{
		// monster, monsterattack, player, , playerattack
			vector<unordered_map<UINT, W::GameObject*>> vecObjects;
		W::Scene* pScene = W::SceneManger::FindScene(_iSceneID);
		vecObjects.reserve(6);

		Protocol::S_MAP pkt;
		auto pMonster = pScene->GetLayer(W::eLayerType::Monster)->GetGameObjects();
		auto pMonsterAttack = pScene->GetLayer(W::eLayerType::MonsterAttack)->GetGameObjects();
		auto pPlayer = pScene->GetLayer(W::eLayerType::Player)->GetGameObjects();
		pPlayer.erase(_iPlayerID);
		auto pPlayerAttack = pScene->GetLayer(W::eLayerType::AttackObject)->GetGameObjects();
		auto pObject = pScene->GetLayer(W::eLayerType::Object)->GetGameObjects();
		auto pUI = pScene->GetLayer(W::eLayerType::UI)->GetGameObjects();

		vecObjects.push_back(move(pMonster));
		vecObjects.push_back(move(pMonsterAttack));
		vecObjects.push_back(move(pPlayer));
		vecObjects.push_back(move(pPlayerAttack));
		vecObjects.push_back(move(pObject));
		vecObjects.push_back(move(pUI));

		for (int i = 0; i < vecObjects.size(); ++i)
		{
			auto iter = vecObjects[i].begin();
			for (iter; iter != vecObjects[i].end(); ++iter)
			{
				Protocol::ObjectInfo tInfo = {};
				W::GameObject* pGameObj = iter->second;

				W::eLayerType eLayer = pGameObj->GetLayerType();
				UINT iCreateID = pGameObj->GetCreateID();
				UINT iObjectID = pGameObj->GetObjectID();
				tInfo.set_scene_layer_createid_id((UCHAR)_iSceneID << 24 | (UCHAR)eLayer << 16 | iCreateID << 8 | iObjectID);
				//tInfo.set_state();

				Transform* pTr = pGameObj->GetComponent<Transform>();
				const Vector3 vPosition = pTr->GetPosition();
				const Vector3 vRotation = pTr->GetRotation();
				Protocol::TransformInfo* tTrInfo = tInfo.mutable_transform();

				tTrInfo->set_p_x(vPosition.x);	tTrInfo->set_p_y(vPosition.y);	tTrInfo->set_p_z(vPosition.z);
				tTrInfo->set_r_x(vRotation.x);	tTrInfo->set_r_y(vRotation.y);	tTrInfo->set_r_z(vRotation.z);

				if (pGameObj->IsPoolObject())
					tInfo.set_object_name(WstringToString(pGameObj->GetName()));

				UCHAR cDir = 1;
				CHAR cAnimIdx = 0;
				UCHAR bRender = pGameObj->IsRender();
				W::Animator* pAnim = pGameObj->GetComponent<W::Animator>();
				if (pAnim)
					cAnimIdx = 0;

				tInfo.set_state_value((bRender << 16) | (cDir << 8) | cAnimIdx);

				*pkt.add_objinfo() = tInfo;
			}
		}

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.GetPersonByID(_iPlayerID)->Send(pSendBuffer);

	}

	

}
