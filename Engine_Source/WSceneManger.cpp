#include "WSceneManger.h"

#include "WRigidbody.h"
#include "WAnimator.h"


#include "..\Engine\WPlayer.h"
#include "..\Engine\WBattleManager.h"

#include "..\Engine\WPlayerAttackObject.h"
#include "..\Engine\WObjectPoolManager.h"
#include "..\Engine\WMonsterManager.h"
#include "..\Engine\WPlayerScript.h"
namespace W
{
	UINT SceneManger::SCENE_IDX = 0;
	std::unordered_map<std::wstring, Scene*> SceneManger::m_hashScene = {};
	std::unordered_map<std::wstring, std::vector<UINT>> SceneManger::m_hashPlayerScene = {};

	void SceneManger::Initialize()
	{
		

	}
	void SceneManger::Update()
	{
		auto iter = m_hashPlayerScene.begin();
		for (iter; iter != m_hashPlayerScene.end(); ++iter)
		{
			if (iter->second.size() > 0)
			{
				m_hashScene.find(iter->first)->second->Update();
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
				m_hashScene.find(iter->first)->second->LateUpdate();
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
				m_hashScene.find(iter->first)->second->UpdatePacket();
			}
		}
	}

	void SceneManger::Release()
	{
		for (auto &iter : m_hashScene)
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
		const wstring& strSceneName = _pGameObject->GetSceneName();

		Scene* pScene = FindScene(strSceneName);
		pScene->EraseObject(eLayer, _pGameObject);

		Protocol::S_DELETE pkt;
		UINT iObjectID = _pGameObject->GetObjectID();

		pkt.set_pool_object(_pGameObject->IsPoolObject());
		pkt.set_layer_deleteid((((UCHAR)eLayer) << 24) | iObjectID);

		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(strSceneName));
	}

	Scene* SceneManger::FindScene(const std::wstring& _strSceneName)
	{
		const auto& iter = m_hashScene.find(_strSceneName);
		if(iter == m_hashScene.end())
			return nullptr;

		return iter->second;
	}

	Scene* SceneManger::GetActiveScene(GameObject* _pGameObj)
	{
		const std::wstring& strSceneName = _pGameObj->GetSceneName();
		Scene* pScene = FindScene(strSceneName);

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
				vecPlayerScene.push_back(m_hashScene.find(iter->first)->second);
			}
		}
		return vecPlayerScene;
	}

	void SceneManger::AddGameObject(const std::wstring& _strSceneName, eLayerType _eType, GameObject* _pGameObj)
	{
		Scene* pScene = FindScene(_strSceneName);
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

	GameObject* SceneManger::FindPlayer(const std::wstring& strSceneName, UINT _iPlayerID)
	{
		Scene* pScene = FindScene(strSceneName);
		if(pScene == nullptr)
			return nullptr;
		
		GameObject* pGameObj = 
			pScene->GetLayer(eLayerType::Player)->GetGameObjects().find(_iPlayerID)->second;

		return pGameObj;
	}

	GameObject* SceneManger::FindPlayer(const std::wstring& _strSceneName)
	{
		Scene* pScene = m_hashScene[_strSceneName];
		std::unordered_map<UINT, GameObject*> hashPlayer = 
			pScene->GetLayer(eLayerType::Player)->GetGameObjects();

		if(hashPlayer.empty())
			return nullptr;
		
		srand(time(nullptr));

		int iPlayerCount = hashPlayer.size();
		int iNumber = rand() % iPlayerCount;

		auto iter = hashPlayer.begin();
		for (int i = 0; i < iNumber; ++i)
			++iter;

		return iter->second;
	}

	std::vector<GameObject*> SceneManger::GetPlayers(const std::wstring& _strSceneName)
	{
		std::vector<GameObject*> vecPlayer;

		const std::unordered_map<UINT, GameObject*>& hashObjs
			= m_hashScene[_strSceneName]->GetLayer(eLayerType::Player)->GetGameObjects();

		auto iter = hashObjs.begin();
		for (iter; iter != hashObjs.end(); ++iter)
		{
			vecPlayer.push_back(iter->second);
		}
		return vecPlayer;
	}

	std::vector<UINT> SceneManger::GetPlayerIDs(const std::wstring& _strSceneName)
	{
		return m_hashPlayerScene[_strSceneName];
	}

	void SceneManger::SwapObject(Scene* _pPrevScene, Scene* _pNextScene, GameObject* _pGameObject)
	{
		eLayerType eType = _pGameObject->GetLayerType();
		
		_pPrevScene->EraseObject(eType, _pGameObject);
		_pNextScene->AddGameObject(eType, _pGameObject);
	}


	void SceneManger::SwapPlayer(GameObject* _pPlayer, const wstring& _strPrevScene, const wstring& _strNextScene)
	{
		Vector3 vPos = _pPlayer->GetComponent<Transform>()->GetPosition();
		vPos.x = 0.f; vPos.y = 0.f;
		_pPlayer->GetComponent<Transform>()->SetPosition(vPos);
		_pPlayer->GetComponent<Rigidbody>()->SetGround(false);
		
		Scene* pNextScene = FindScene(_strNextScene);
		pNextScene->AddGameObject(eLayerType::Player, _pPlayer);

		UINT iPlayerID = _pPlayer->GetObjectID(); //obj == playerid
		auto& iterPrev = m_hashPlayerScene[_strPrevScene];
		iterPrev.erase(
			std::remove(iterPrev.begin(), iterPrev.end(), iPlayerID),
			iterPrev.end()
		);

		// 다음 씬에 플레이어 ID 추가
		m_hashPlayerScene[_strNextScene].push_back(iPlayerID);
	}


	void SceneManger::PushObjectPool(UINT _iPlayerID, Scene* _pPrevScene)
	{
		const std::unordered_map<UINT, GameObject*>& hashObjs
			= _pPrevScene->GetLayer(eLayerType::AttackObject)->GetGameObjects();

		auto iter = hashObjs.begin();
		for (iter; iter != hashObjs.end(); ++iter)
		{
			PlayerAttackObject* pAttackObj = static_cast<PlayerAttackObject*>(iter->second);
			if (pAttackObj->GetPlayer()->GetPlayerID() == _iPlayerID)
				pAttackObj->Off();

		}
	}

	void SceneManger::AddPlayerScene(Player* pPlayer, const std::wstring& _strScene)
	{
		AddGameObject(_strScene, eLayerType::Player, pPlayer);

		m_hashPlayerScene[_strScene].push_back(pPlayer->GetPlayerID());
	}

	void SceneManger::SendEnterScene(UINT _iPlayerID, const std::wstring& _strNextScene)
	{
		Protocol::S_MAP pkt;

		// monster, monsterattack, player, , playerattack
			vector<unordered_map<UINT, W::GameObject*>> vecObjects;
		W::Scene* pScene = W::SceneManger::FindScene(_strNextScene);
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
				tInfo.set_layer_createid_id((UCHAR)eLayer << 24 | iCreateID << 16 | iObjectID);

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
