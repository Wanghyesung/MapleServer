#pragma once

//#include "Engine.h"
#include "WGraphics.h"
#include "WTexture.h"
//DirectX 11 ���̺귯�� �߰�(����)


namespace W::graphics
{
	class GraphicDevice_Dx11
	{
	
	public:
		GraphicDevice_Dx11();
		~GraphicDevice_Dx11();

		bool CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* _desc, HWND _hwnd);
		bool CreateTexture2D(const D3D11_TEXTURE2D_DESC* _desc, void* _pdata , ID3D11Texture2D** _ppTexture2D);
		bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* _pInputElementDescs, UINT _iNumElements, ID3DBlob* _byteCode, ID3D11InputLayout** _ppInputLayout);
		bool CreateBuffer(ID3D11Buffer** _buffer, D3D11_BUFFER_DESC* _desc, D3D11_SUBRESOURCE_DATA* _data);
		bool CompileFromfile(const std::wstring& _wstrFileName, const std::string& _strFunName, const std::string& _strVersion, ID3DBlob** _ppCod);
		bool CreateVertexShader(const void* _pShaderByteCode, SIZE_T _BytecodeLength, ID3D11VertexShader** _ppVertexShader);
		bool CreateGeometryShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11GeometryShader** _ppGeometryShader);
		bool CreatePixelShader(const void* _pShaderByteCode, SIZE_T _BytecodeLength, ID3D11PixelShader** _ppPixelShader);
		bool CreateComputeShader(const void* _pShaderByteCode, SIZE_T _BytecodeLength, ID3D11ComputeShader** _ppComputeShader);
		bool CreateSampler(const D3D11_SAMPLER_DESC* _pSamplerDesc, ID3D11SamplerState** _ppSamplerState);
		bool CreateRasterizeState(const D3D11_RASTERIZER_DESC* _pRasterizerDesc, ID3D11RasterizerState** _ppRasterizerState);
		bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* _pDepthStencilDesc, ID3D11DepthStencilState** _ppDepthStencilState);
		bool CreateBlendState(const D3D11_BLEND_DESC* _pBlendStateDesc, ID3D11BlendState** _ppBlendState);
		bool CreateShaderResourceView(ID3D11Resource* _pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc, ID3D11ShaderResourceView** _ppSRVView);
		bool CreateDepthStencilView(ID3D11Resource* _pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc, ID3D11DepthStencilView** _ppDepthStencilView);
		bool CreateRenderTargetView(ID3D11Resource* _pResource, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc, ID3D11RenderTargetView** _ppRTView);
		bool CreateUnorderedAccessView(ID3D11Resource* _pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* _pDesc, ID3D11UnorderedAccessView** _ppUAView);

		void BindInputLayout(ID3D11InputLayout* _pInputLayout);
		void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology);
		void BindVertexBuffer(UINT _iStartSlot, ID3D11Buffer* const* _ppVertexBuffers, const UINT* _pStrides, const UINT* _pOffsets);
		void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT _Format, UINT _iOffset);
		void BindVertexShader(ID3D11VertexShader* _pVertexShader);
		void BindHullShader(ID3D11HullShader* _pHullShader);
		void BindDomainShader(ID3D11DomainShader* _pDomainShader);
		void BindGeometryShader(ID3D11GeometryShader* _pGeometryShader);
		void BindPixelShader(ID3D11PixelShader* _pPixelShader);
		void BindComputeShader(ID3D11ComputeShader* _pComputeShader);
		void Dispatch(UINT _iThreadGroupX, UINT _iThreadGroupY, UINT _iThreadGroupZ);

		void SetConstantBuffer(ID3D11Buffer* _buffer, void* _data, UINT _iSize);
		void BindConstantBuffer(eShaderStage _eStage, eCBType _eType, ID3D11Buffer* _buffer);
		void BindBuffer(ID3D11Buffer* _pBuffer, void* _data, UINT _iSize);
		void BindsConstantBuffer(eShaderStage _eStage, eCBType _eType, ID3D11Buffer* _buffer);
		void BindShaderResource(eShaderStage _eStage, UINT _iStartSlot, ID3D11ShaderResourceView** _ppSRV);
		void BinUnorderedAccess(UINT _iSolt, ID3D11UnorderedAccessView** _ppUnorderedAccessViews, const UINT* _pUAVInitialCounts);

		void BindSampler(eShaderStage _eStage, UINT _iStartSlot, ID3D11SamplerState** _ppSamplers);
		void BindViewPort(D3D11_VIEWPORT* _viewPort);//����Ʈ�� ��ȯ
		void BindRasterizeState(ID3D11RasterizerState* _pRasterizerState);
		void BindDepthStencilState(ID3D11DepthStencilState* _pDepthStencilState);
		void BindBlendState(ID3D11BlendState* _pBlendState);
		void CopyResource(ID3D11Resource* _pDstResource, ID3D11Resource* _pSrcResource);

		void DrawIndexed(UINT _iIndexCount, UINT _iStartIndexLocation, INT _iBaseVertexLocation);
		void DrawIndexedInstanced(UINT _iIndexCountPerInstance, UINT _iInstanceCount
			, UINT _iStartIndexLocation, INT _iBaseVertexLocation, UINT _iStartInstanceLocation);
		void ClearTarget();
		void UpdateViewPort();
		void Draw();
		void Present();
		
		ID3D11Device* GetID3D11Device() { return m_cpDevice.Get(); }
		ID3D11DeviceContext* GetID3D11DeviceContext() { return m_cpContext.Get(); }
	private:
		// ���� �׷���ī�� �ϵ���� ��ü
		Microsoft::WRL::ComPtr<ID3D11Device> m_cpDevice;

		//  dx11���� ���������� ����̽���ü ���������ʰ�
		// �̰�ü�� �̿��Ͽ� ����� ������.
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_cpContext;

		// ���������� �׷��� �ؽ�ó(��ȭ��)
		//Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cpRenderTarget;
		//
		//// ����Ÿ�ٿ� ������������ �ʰ� ����Ÿ�ٺ並 ���ؼ� �����Ѵ�.
		//Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_cpRenderTargetView;
		//
		//// ���̹���
		//Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cpDepthStencilBuffer;
		//
		//// ���̹��ۿ� �����Ҽ� �ִ� ��
		//Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_cpDepthStencilView;
		std::shared_ptr<W::graphics::Texture> m_cpRenderTarget;
		std::shared_ptr<W::graphics::Texture> m_cpDepthStencil;

		// ������۸� �۾��� �������ִ� swapChain
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_cpSwapChain;

		D3D11_VIEWPORT m_tViewPort;
	};

	inline GraphicDevice_Dx11*& GetDevice()
	{
		static GraphicDevice_Dx11* device = nullptr;
		return device;
 	}
}

