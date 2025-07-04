#pragma once

#include "..\Engine_Source\WScene.h"

namespace W
{
	class ValleyScene : public Scene
	{
	public:
		ValleyScene();
		virtual ~ValleyScene();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
	
		virtual void OnEnterPlayer(UINT _iPlayerID)override;
		virtual void OnExitPlayer(UINT _iPlayerID)override;

		virtual void OnEnter();
		virtual void OnExit();

		virtual void CreateBackground() override;
		

	private:
		void setobject();
		void setmonster();
	};
}




