#pragma once
#include "WGameObject.h"

namespace W
{
	class Ladder : public GameObject
	{
	public:
		Ladder();
		virtual ~Ladder();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		
		DECLARE_CREATE_ID;
	private:


	};
}


