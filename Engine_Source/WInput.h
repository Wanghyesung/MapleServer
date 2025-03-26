#pragma once

#include "Engine.h"
#include "WGraphicDevice_Dx11.h"
namespace W
{
	using namespace W::math;

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
		struct Key
		{
			eKeyCode key;
			eKeyState state;
			bool bPressed;
		};

		static void Initialize();
		static void Update();
		static void Render(HDC hdc);
		
		inline static eKeyState GetKeyState(eKeyCode keyCode)
		{
			return m_vecKeys[(UINT)keyCode].state;
		};

		//GetKey()		Ű�� ������ �ð���ŭ true�� ��ȯ
		//GetKeyDown()	Ű�� ������ ��, �� �ѹ� true�� ��ȯ
		//GetKeyUp()	Ű�� ������ ���� ��, �� �ѹ� true�� ��ȯ

		static __forceinline bool GetKey(eKeyCode keyCode)
		{
			return m_vecKeys[static_cast<UINT>(keyCode)].state == eKeyState::Pressed;
		}

		static __forceinline bool GetKeyDown(eKeyCode keyCode)
		{
			return m_vecKeys[static_cast<UINT>(keyCode)].state == eKeyState::Down;
		}

		static __forceinline bool GetKeyUp(eKeyCode keyCode)
		{
			return m_vecKeys[static_cast<UINT>(keyCode)].state == eKeyState::Up;
		}

		static __forceinline Vector2 GetMousePos() { return m_vMousePos; }

	private:
		static std::vector<Key> m_vecKeys;
		static Vector2 m_vMousePos;
	};

}


