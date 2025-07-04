#pragma once
#include "..\Engine_Source\WScene.h"

namespace W
{
	class TempleBossScene : public Scene
	{
	public:
		TempleBossScene();
		virtual ~TempleBossScene();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		
		virtual void OnEnterPlayer(UINT _iPlayerID)override;
		virtual void OnExitPlayer(UINT _iPlayerID)override;

		virtual void OnEnter()override;
		virtual void OnExit()override;

		virtual void CreateBackground() override;


	private:
		void create_monster();
		void create_effect();
	};

}

