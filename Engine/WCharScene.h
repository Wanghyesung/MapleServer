#pragma once
#include "..\Engine_Source\WScene.h"

namespace W
{
	class CharScene : public Scene
	{
	public:
		CharScene();
		virtual ~CharScene();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
		virtual void OnEnter()override;
		virtual void OnExit()override;

		virtual void CreateBackground()override;

	private:


	};

}

