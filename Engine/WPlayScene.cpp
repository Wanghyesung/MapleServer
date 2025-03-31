#include "WPlayScene.h"
#include "WTransform.h"
#include "WResources.h"
#include "WCameraScript.h"
#include "WObject.h"
#include "WPlayer.h"
#include "WCollider2D.h"
#include "WPlayerScript.h"
#include "WRigidbody.h"
#include "WCollisionManager.h"
#include "WAnimator.h"
#include "WLight.h"
#include "WAnimator.h"
#include "WParticleSystem.h"
namespace W
{
	PlayScene::PlayScene()
	{
		SetMapSize(-10.f, 5.f, 10.f, -5.f);
		SetMapPossibleSize(-10.f, 10.f);
	}
	PlayScene::~PlayScene()
	{
	}
	void PlayScene::Initialize()
	{
		CollisionManager::SetLayer(this, eLayerType::Player, eLayerType::Monster, true);

		{
			GameObject* player = new GameObject();
			player->SetName(L"Particle");
			AddGameObject(eLayerType::Monster, player);
			//ParticleSystem* mr = player->AddComponent<ParticleSystem>();
			player->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 1.0f));
			player->GetComponent<Transform>()->SetScale(Vector3(0.2f, 0.2f, 0.2f));
			
		}

		{
			GameObject* light = new GameObject();
			light->SetName(L"Smile");
			AddGameObject(eLayerType::Light, light);
			Light* lightComp = light->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		{
			GameObject* light = new GameObject();
			light->SetName(L"Smile");
			AddGameObject(eLayerType::Light, light);
			Light* lightComp = light->AddComponent<Light>();
			lightComp->SetType(eLightType::Point);
			lightComp->SetColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
			lightComp->SetRadius(3.0f);
		}
		
	}
	void PlayScene::Update()
	{
		Scene::Update();
	}
	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	
}
