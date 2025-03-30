#pragma once
#include "..\Engine_Source\WScene.h"
namespace W
{
	class HelisiumBoss : public Scene
	{
	public:
		HelisiumBoss();
		virtual ~HelisiumBoss();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
	
		virtual void OnEnter()override;
		virtual void OnExit()override;

		virtual void CreateBackground() override;

	private:
		void create_monster();
		void create_effect();
	};
}


