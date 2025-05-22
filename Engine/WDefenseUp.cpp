#include "WDefenseUp.h"
#include "WResources.h"
#include "WMonster.h"

namespace W
{
	UINT DefenseUp::CREATE_ID = 0;
	DefenseUp::DefenseUp(float _fAccStat) :
		UpStat(_fAccStat)
	{
		GetComponent<Transform>()->SetScale(1.f * 0.19f, 1.f * 0.2f, 0.f);

	}
	DefenseUp::~DefenseUp()
	{

	}
	void DefenseUp::Initialize()
	{

	}
	void DefenseUp::Update()
	{
		UpStat::Update();
	}
	void DefenseUp::LateUpdate()
	{
		UpStat::LateUpdate();
	}
	//void DefenseUp::Render()
	//{
	//	UpStat::Render();
	//}
}