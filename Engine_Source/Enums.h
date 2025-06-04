#pragma once


namespace W
{
	enum class eComponentType
	{
		None,
		Transform,
		Mesh,
		Collider2D,
		Rigidbody,
		Particle,
		MeshRenderer,
		Animator,
		Script,
		Light,
		AudioListener,
		AudioSource,
		End,
	};

	enum class eLayerType
	{
		Default,
		Camera,
		Background,
		Player,
		Ladder,
		Ground,
		NPC,
		Object,
		Box,
		Monster,
		MonsterAttack,
		AttackObject,
		Effect,
		ItemObject,
		Light,
		UI,
		End,
	};

	enum class eResourceType
	{	
		Animation,
		End,
	};

	enum class eCameraType
	{
		Object,
		UI,
		None,
	};

	enum class eParentUI
	{
		Interface,
		Inventory,
		SkillStorage,
		EquipState,
		None,
	};
	
	enum class eColliderType
	{
		Rect,
		Circle,
		Sphere,
		Cube,
		End,
	};

	enum class eLightType
	{
		Directional,
		Point,
		FadeOut,
		Spot,
		End,
	};

	
}
