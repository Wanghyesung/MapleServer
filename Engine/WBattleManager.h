#pragma once
#include "WAbnormal.h"
#include "..\Engine_Source\WInfo.h"
#include "WEffect.h"

namespace W
{
	//SceneManager 다음 실행

	class BattleManager
	{
	public:
		enum class eAbnormalType
		{
			None,
			SealSkill,
			temptation,
			Faint,
			Confusion,
			Slow,
			Stop,
			Undead,
			DemianStop,
			InputKey,
			Stigma,
			End
		};

		enum class eUpStatType
		{
			None,
			Attack,
			Speed,
			Defense,
			Heal,
			Ignore,
			ReflexAttack,
			AccHP,
			AccMP,
			End
		};

		struct tDamageCount
		{
			tDamageCount() :
				m_fResetTime(0.2f), m_fCurTime(0.f) {}
			tDamageCount(UINT _iCurCount, UINT _iEndCount) :
				m_fResetTime(0.2f), iCurCount(_iCurCount), iEndCount(_iEndCount), m_fCurTime(0.f) {}

			UINT iCurCount;
			UINT iEndCount;

			float m_fCurTime;
			float m_fResetTime;
		};

		static void Initialize();
		static void Release();
		static void Update();
		static void CheckDamage(tObjectInfo& _tObjectInfo, const tAttackInfo& _tAttackInfo, 
			const std::wstring _strName, const Vector3& _vPosition, UINT _iSceneID, UINT _iPlayerID =0);
		
		static UINT GetMaxDamage() { return m_iMaxDamage; }
		static UINT GetStigmaCount(UINT _iPlayerID) { return m_arrStigmaCount[_iPlayerID]; }
		static void SetStigmaCount(UINT _iPlayerID, UINT _iCount) { m_arrStigmaCount[_iPlayerID] = _iCount; }

		static bool IsAblePotion();

		static void PushEffect(Effect* _pEffect);
		static Effect* GetEffect(std::wstring _strName);

		static void HitchAbnormal(GameObject* _pPlayer, eAbnormalType _eType, float _fAccStat = 0.f);
		static void Buff_Stat(GameObject* _pTarget, eUpStatType _eType, float _fAccStat);

		static void Player_DeBuff_Attack(GameObject* _pTarget, eUpStatType _eType, float _fAccStat);

	private:
		static void active_damage(std::queue<class DamageFont*>& _queueFonts, UINT _iDamageCount, 
			const std::wstring& _strName, UINT _iPlayerID = 0);
		static void erase_damage(const std::wstring& _strName, UINT _iPlayerID = 0);
		static tDamageCount& add_damage(UINT _iDamageCount, const std::wstring& _strName, UINT _iPlayerID = 0);


		static void excute_abnormal(eAbnormalType _eType, GameObject* _pTarget, float _fAccValue);
		static void excute_stat(eUpStatType _eType, GameObject* _pTarget, float _fAccValue);

		static void stop(GameObject* _pGameObject ,float _fAccValue);
		static void faint(GameObject* _pGameObject, float _fAccValue);
		static void seal_skill(GameObject* _pGameObject, float _fAccValue);
		static void temptation(GameObject* _pGameObject, float _fAccValue);
		static void confusion(GameObject* _pGameObject, float _fAccValue);//변이
		static void undead(GameObject* _pGameObject, float _fAccValue);
		static void demianstop(GameObject* _pGameObject, float _fAccValue);
		static void inputkey(GameObject* _pGameObject, float _fAccValue);
		static void stigma(GameObject* _pGameObject, float _fAccValue);

		//상태이상 해제
		static void restore(GameObject* _pTarget, eAbnormalType _eType, float _fAccValue = 0.f);
		//static void restore_move(Player* _pTarget);

		//monster
		static void buff_attack(GameObject* _pTarget, float _fAccDamage);
		static void buff_defense(GameObject* _pTarget, float _fDefense);
		static void buff_ignore(GameObject* _pTarget, float _fsign);
		static void buff_heal(GameObject* _pTarget, float _fAccHeal);
		static void buff_reflex(GameObject* _pTarget, float _fsign);

		//player
		static void buff_speed(GameObject* _pTarget, float _fAccSpeed);
		static void debuff_slow(GameObject* _pGameObject,float _fAccSpeed);
		static void buff_HP(GameObject* _pTarget, float _fAccValue);
		static void buff_MP(GameObject* _pTarget, float _fAccValue);

	private:
		static UINT m_iMaxDamage;

		static std::function<void(GameObject* _pTarget, float _fAccValue)> m_arrAbnormalFunc[(UINT)eAbnormalType::End];
		static std::function<void(GameObject* _pTarget, float _fAccValue)> m_arrStatFunc[(UINT)eUpStatType::End];

		static std::map<std::wstring, std::queue<Effect*>> m_mapEffects;
		static std::map<std::wstring, tDamageCount> m_mapDamage[6];

		//상태이상 
		static bool m_bOnAbnormal;
		static bool m_bOnUndead;
		//체력 회복 시간
		static float m_fPotionTime;
		static float m_fCurPotionTime;

		static UINT m_arrStigmaCount[6];

		friend class EventManager;
		friend class Abnormal;
	};
}


