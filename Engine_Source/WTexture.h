#pragma once

#include "WResource.h"
#include "WGraphics.h"

#include "..\External\DirectXTex\Include\DirectXTex.h"
#include "..\External\DirectXTex\Include\DirectXTex.inl"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\DirectXTex\\Lib\\Debug\\DirectXTex.lib")
#else
#pragma comment(lib, "..\\External\\DirectXTex\\Lib\\Release\\DirectXTex.lib")
#endif

//gpu�� ���, �� gpu�� Ȱ���ϰ�
//��ǻƮ���̴� ���ٿ� �ʿ�  

namespace W::graphics
{
	class Texture : public Resource
	{
	public:
		Texture();
		virtual ~Texture();

		bool Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _format, UINT _iBindFlag);
		virtual HRESULT Load(const std::wstring& _strPath);
		void BindShaderResource(eShaderStage _eStage, UINT _iStartSlot);
		void BindUnorderedAccessViews(UINT _iSlot);
		void ClearUnorderedAccessViews(UINT _iSlot);
		void Clear();

		size_t GetWidth() { return m_iWidth; }
		size_t GetHeight() { return m_iHeight; }

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    GetRTV() { return  m_cpRTV; }
		void SetRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv) { m_cpRTV = rtv; }
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>    GetDSV() { return  m_cpDSV; }
		void SetDSV(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv) { m_cpDSV = dsv; }
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  GetSRV() { return  m_cpSRV; }
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> GetUAV() { return  m_cpUAV; }
		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture() { return m_cpTexture; }
		void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture) { m_cpTexture = texture; }

	private:
		//scratchImage�����̹����� �������� ����ִ� Ȯ����
		ScratchImage m_Image;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cpTexture;//2d�̹���
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cpSRV;//shader������ view
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_cpRTV;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_cpDSV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_cpUAV;
		UINT m_iWidth;
		UINT m_iHeight;

		D3D11_TEXTURE2D_DESC m_tDesc;
	};

	//�ؽ���(uv��ǥ)�� �ȼ��� �ϳ��ϳ� �Űܰ�����(2dTexture)
	//�ؽ��ĵ� �ٸ� �׷��Ƚ� ��ü�� ���� view�� ���ؼ� ���� shader���� �����ϴ� view(shaderview)�� ���� ���� �ؽ��İ� shader�� ���ؼ� ��µǱ� ������ 
	//shader���� ������ۿ� ���� slot�� ������ ���۴�b �ؽ��Ĵ� t�� ����
}


