#include "WInput.h"

#include "WApplication.h"


extern W::Application application;

namespace W
{
	int ASCII[(UINT)eKeyCode::NONE] =
	{
		VK_LSHIFT,//LSHIFT,
		VK_INSERT,//ins
		VK_HOME,//home
		VK_PRIOR, //page up
		VK_CONTROL,//CTRL,
		VK_DELETE, //delete,
		VK_END,//end,
		VK_NEXT,//pdn,

		0x30,
		0x31,
		0x32,
		0x33,
		0x34,
		0x35,
		0x36,
		0x37,
		0x38,
		0x39,

		'Q', 'W', 'E', 'R', 'T',
		'A', 'S', 'D', 'F', 'G',

		'Z', 'X', 'C', 'V', 'B', 'N', 'M',

		'Y', 'U', 'I', 'O', 'P',
		'H', 'J', 'K', 'L',

		VK_RETURN,//ENTER,
		VK_LMENU,//ALT,
		VK_ESCAPE,//ESC,
		VK_SPACE,

		VK_UP, VK_DOWN, VK_LEFT ,VK_RIGHT,
		VK_LBUTTON, VK_RBUTTON,

		//WM_VSCROLL
		 VK_BACK,
	};


	std::vector<eKeyState> Input::m_vecKeys[MAXCOUNT] = {};
	std::queue<UCHAR> Input::m_queueKeys[MAXCOUNT][(UINT)eKeyCode::NONE + 1] = {};

	void Input::Initialize()
	{
		for (int i = 0; i < MAXCOUNT; ++i)
		{
			for (UINT j = 0; j < (UINT)eKeyCode::NONE; j++)
			{
				m_vecKeys[i].push_back(eKeyState::None);
			}
		}
	}

	void Input::Update()
	{
		for (int i = 0; i < MAXCOUNT; ++i)
		{
			for (int j = 0; j <= (UINT)eKeyCode::NONE; j++)
			{
				if (!m_queueKeys[i][j].empty())
				{
					UCHAR eState = m_queueKeys[i][j].front();
					m_queueKeys[i][j].pop();
					m_vecKeys[i][j] = (eKeyState)eState;
				}
			}
		}

	}

	void Input::Update_Key(UINT _iPlayerID, const std::vector<USHORT>& _vecKey)
	{
		for (UINT i = 0; i < _vecKey.size(); ++i)
		{
			USHORT sKey = _vecKey[i];
			// 상위 8비트는 key, 하위 8비트는 상태
			UCHAR iKey = (sKey >> 8) & 0xFF;     
			UCHAR eState = sKey & 0xFF;
			
			m_queueKeys[_iPlayerID][iKey].push(eState);
		}

	}


}

