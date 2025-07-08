#include "WIgnore.h"
#include "WResources.h"
#include "WMonster.h"

namespace W
{
	UINT Ignore::CREATE_ID = 0;

	Ignore::Ignore(float _fAccStat) :
		UpStat(_fAccStat)
	{
		
		GetComponent<Transform>()->SetScale(1.f * 0.34f, 1.f * 0.34f, 0.f);

	}
	Ignore::~Ignore()
	{

	}
	void Ignore::Initialize()
	{

	}
	void Ignore::Update()
	{
		UpStat::Update();
	}
	void Ignore::LateUpdate()
	{
		UpStat::LateUpdate();
	}

}