#pragma once
#include "Engine.h"

namespace W
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		void Initialize();
		void Update();
		void LateUpdate();
		void UpdatePacket();
		void SetWindow(HWND _hHwnd, UINT _iWidth, UINT _iHeight);

		UINT GetWidth() { return m_iWidth; }
		UINT GetHeight() { return m_iHeight; }
		HWND GetHwnd() { return m_hHwnd; }

	private:
		bool mbInitialize = false;
		// 오로지 한개의 객체만 만들수 있는 스마트 포인터
		//std::unique_ptr<W::graphics::GraphicDevice_Dx11> graphicDevice;

		// HDC mHdc; -> GPU API
		HWND m_hHwnd;
		UINT m_iWidth;
		UINT m_iHeight;

	};
}

