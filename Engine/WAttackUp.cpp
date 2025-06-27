#include "WAttackUp.h"
#include "WResources.h"
#include "WMonster.h"

namespace W
{
	UINT AttackUp::CREATE_ID = 0;

	AttackUp::AttackUp(float _fAccStat):
		UpStat(_fAccStat)
	{
		
		GetComponent<Transform>()->SetScale(1.f * 0.19f,1.f * 0.23f,0.f);

	}
	AttackUp::~AttackUp()
	{

	}
	void AttackUp::Initialize()
	{

	}
	void AttackUp::Update()
	{
		UpStat::Update();
	}
	void AttackUp::LateUpdate()
	{
		UpStat::LateUpdate();
	}
	void AttackUp::UpdatePacket()
	{
		UpStat::UpdatePacket();
	}
	
}