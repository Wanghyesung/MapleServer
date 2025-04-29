#pragma once

#include "Engine.h"

namespace W
{
	
	enum class eKeyCode
	{
		LSHIFT,
		INS,
		HOME,
		PGUP,
		CTRL,
		Del,
		END,
		PDN,

		KEY0, KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, KEY8, KEY9,

		Q, W, E, R, T,
		A, S, D, F, G,	

		Z, X, C, V, B, N, M,

		ENTER,
		ALT,
		ESC,
		SPACE,

		UP, DOWN, LEFT, RIGHT, 
		LBUTTON, RBUTTON,

		//SCROLL,

		NONE = 46,
	};

	enum class eKeyState
	{
		Down,
		Pressed,
		Up,
		None,
	};

	class Input
	{
	public:

		static void Initialize();
		static void Update();
		static void Update_Key(UINT _iPlayerID, const std::vector<USHORT>& _vecKey);


		inline static eKeyState GetKeyState(UINT _iPlayerID, eKeyCode keyCode)
		{
			return m_vecKeys[_iPlayerID][static_cast<UINT>(keyCode)];
		};

		static __forceinline bool GetKey(UINT _iPlayerID, eKeyCode keyCode)
		{
			return m_vecKeys[_iPlayerID][static_cast<UINT>(keyCode)] == eKeyState::Pressed;
		}

		static __forceinline bool GetKeyDown(UINT _iPlayerID, eKeyCode keyCode)
		{
			return m_vecKeys[_iPlayerID][static_cast<UINT>(keyCode)] == eKeyState::Down;
		}

		static __forceinline bool GetKeyUp(UINT _iPlayerID, eKeyCode keyCode)
		{
			return m_vecKeys[_iPlayerID][static_cast<UINT>(keyCode)] == eKeyState::Up;
		}

		
		//최대 5명의 플레이어 Input정보
	private:
		static std::queue<UCHAR> m_queueKeys[6][(UINT)eKeyCode::NONE + 1];//지금까지 들어온 키 처리
		static std::vector<eKeyState> m_vecKeys[6];//현 체크할 키
	};

}


