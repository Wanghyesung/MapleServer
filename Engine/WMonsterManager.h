#pragma once
#include "Engine.h"

namespace W
{
	class Monster;
	class GameObject;

	class MonsterManager
	{
	public:
		static void Update();
		static void AddDeadObject(Monster* _pGameObj);
		static void AddDeleteObject(GameObject* _pGameObj);
		
	private:
		static void respawn(Monster* _pGameObj);
	
	private:
		static std::vector<Monster*> m_vecDeadObjs;
		static std::vector<GameObject*> m_vecExpectedDeleteObjs;
		static std::vector<float> m_vecReSpwanTime;
	};
}


