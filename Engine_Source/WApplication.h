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
	
		void SetWindow(HWND _hHwnd, UINT _iWidth, UINT _iHeight);

		UINT GetWidth() { return m_iWidth; }
		UINT GetHeight() { return m_iHeight; }
		HWND GetHwnd() { return m_hHwnd; }

	private:
		bool mbInitialize = false;
		// ������ �Ѱ��� ��ü�� ����� �ִ� ����Ʈ ������
		//std::unique_ptr<W::graphics::GraphicDevice_Dx11> graphicDevice;

		// HDC mHdc; -> GPU API
		HWND m_hHwnd;
		UINT m_iWidth;
		UINT m_iHeight;

	};
}

