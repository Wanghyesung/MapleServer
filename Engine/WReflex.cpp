#include "WReflex.h"
#include "WResources.h"
#include "WMonster.h"
#include "WMonsterScript.h"
namespace W
{
	UINT Reflex::CREATE_ID = 0;

	Reflex::Reflex(float _fAccStat):
		UpStat(_fAccStat),
		m_fCurHp(0)
	{
		GetComponent<Transform>()->SetScale(1.f * 0.37f, 1.f * 0.42f, 0.f);

	}
	Reflex::~Reflex()
	{

	}
	void Reflex::Initialize()
	{
		//현재 hp 저장
	}

	void Reflex::Update()
	{
		UpStat::Update();
	}
	void Reflex::LateUpdate()
	{
		UpStat::LateUpdate();
	}
	//void Reflex::Render()
	//{
	//	UpStat::Render();
	//}
}