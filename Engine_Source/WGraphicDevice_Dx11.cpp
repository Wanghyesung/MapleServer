#include "WGraphicDevice_Dx11.h"
#include "WApplication.h"
#include "WRenderer.h"

extern W::Application application;

namespace W::graphics
{
	//ram���� gpu�׷���ī���� �ּҸ� ����Ű�� ������ �÷���
	//�⺻������ ���� ��������
	//getaddres�� �� ������ �ּҸ� ������ �� ����
	//���߹��۸� ���� �Լ��� �� �����Ϳ� ���� �Ҵ��ϰ� ����
	//desc �ɼǱ�� 

	//����ü�� �ȿ� ���۸� ����Ÿ�Ͽ� ����
	//��������
	//�Ӹ�-> �̸� �ؽ��ĵ��� ũ�⺰�� �����δ¹� ->�׶��׋� ������ �� �ʿ䰡����
	
	//������ �ϴ�, ȸ������ �ϱ� -> �������̳� ����̸� ��ü�� �Ǽ��� ���������� ���� �� �Ⱥ��� �� ����

	GraphicDevice_Dx11::GraphicDevice_Dx11()
	{
		//Device, Context ����
		HWND hwnd = application.GetHwnd();
		//�̰� �ؾ��� ����� ���� ���� �� ���� �������� �ٸ��ɷ�
		UINT deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
		//d3d ����̹� ��ɺ� 
		D3D_FEATURE_LEVEL featureLevel = (D3D_FEATURE_LEVEL)0;
		//int a = 10;
		//int* b = &a;
		//int** c = &b;
		//**c;
		//ID3D11Device* pDevice = nullptr;
		//ID3D11DeviceContext* pContext = nullptr;
		//ID3D11DeviceContext** ppContext = &pContext;

		//null����, ����̹����� Ÿ��, ����̹��� HARDWARE�� ��� null,
		//��Ÿ�� ����, featruelevel�� �� D3D11_SDK_VERSION���, 
		D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			deviceFlag, nullptr, 0, D3D11_SDK_VERSION,
			m_cpDevice.GetAddressOf(), &featureLevel,
			m_cpContext.GetAddressOf());


		//����ü�� �ɼ� (���۹� ǥ�� ��� ����)
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		//���� ���
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = application.GetWidth();
		swapChainDesc.BufferDesc.Height = application.GetHeight();

		if (!CreateSwapChain(&swapChainDesc, hwnd))
			return;

		m_cpRenderTarget = std::make_shared<Texture>();
		m_cpDepthStencil = std::make_shared<Texture>();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTarget = {};
		// ����ü�ο� �ִ� ����Ÿ�� ��������
		if (FAILED(m_cpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
			, (void**)renderTarget.GetAddressOf())))
			return;
		m_cpRenderTarget->SetTexture(renderTarget);


		//����Ÿ�ٺ� ����
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
		m_cpDevice->CreateRenderTargetView((ID3D11Resource*)m_cpRenderTarget->GetTexture().Get()
			, nullptr, renderTargetView.GetAddressOf());
		m_cpRenderTarget->SetRTV(renderTargetView);

		//2d �ؽ��� �ɼ�(���̹���)
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		//��� ���� ��������  ���� ���ٽ� ������� ���ε��մϴ�
		depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		//�ؽ�ó�� �а� ���� ����� �ĺ��ϴ� ���Դϴ�.
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.CPUAccessFlags = 0;
		//24��Ʈ
		depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		//���� �ػ�
		depthStencilDesc.Width = application.GetWidth();
		depthStencilDesc.Height = application.GetHeight();
		//�ؽ�ó �迭�� �ؽ�ó ���Դϴ�
		depthStencilDesc.ArraySize = 1;
	
		//�ȼ��� ���� ���� ���Դϴ�.
		//�̹��� ǰ�� �����Դϴ�. ǰ���� �������� ������ �������ϴ� 0 ~ 1
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		depthStencilDesc.MiscFlags = 0;
		//�ؽ�ó�� �ִ� Mipmap ���� ���Դϴ�
		depthStencilDesc.MipLevels = 0;
		
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencolBuffer = nullptr;
		if (!CreateTexture2D(&depthStencilDesc, nullptr, depthStencolBuffer.GetAddressOf()))
			return;
		m_cpDepthStencil->SetTexture(depthStencolBuffer);

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;
		if (!CreateDepthStencilView(depthStencolBuffer.Get(), nullptr, DepthStencilView.GetAddressOf()))
			return;
		m_cpDepthStencil->SetDSV(DepthStencilView);


		//ndc��ǥ -> window ��ǥ�� ��ȯ(����Ʈ��ȯ)(�»�� ���ϴ� ũ��, 
		//����Ʈ�� ��� �����ؾ� ��ũ���� �ٲ� ���� �ٲ�
		RECT winRect = {};
		GetClientRect(hwnd, &winRect);

		m_tViewPort =
		{
			0.f, 0.f,
			(float)(winRect.right - winRect.left),
			(float)(winRect.bottom - winRect.top),
			0.f,1.f
		};

		BindViewPort(&m_tViewPort);
		// ���ٽ� ���۸� ���-���� �ܰ迡 ���ε��մϴ�.
		m_cpContext->OMSetRenderTargets(1, m_cpRenderTarget->GetRTV().GetAddressOf(), m_cpDepthStencil->GetDSV().Get());

	}
	GraphicDevice_Dx11::~GraphicDevice_Dx11()
	{

	}
	bool GraphicDevice_Dx11::CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* _desc, HWND _hwnd)
	{
		DXGI_SWAP_CHAIN_DESC dxGiDesc = {};

		//���â window�ڵ鰪
		dxGiDesc.OutputWindow = _hwnd;
		//���â �������
		dxGiDesc.Windowed = true;

		dxGiDesc.BufferCount = _desc->BufferCount;
		dxGiDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;

		//ǥ�� �Ǵ� ���ҽ��� ��� ������ ������� ����մϴ�.
		dxGiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxGiDesc.BufferDesc.Width = _desc->BufferDesc.Width;
		dxGiDesc.BufferDesc.Height = _desc->BufferDesc.Height;
		//888 rgb 32��Ʈ
		dxGiDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		dxGiDesc.BufferDesc.RefreshRate.Numerator = 240;//�ִ� ������
		dxGiDesc.BufferDesc.RefreshRate.Denominator = 1; //�ּ� ������
		//������ ������� �ػ󵵿� �°� �̹����� �þ�� ��� (Ȯ����� ����) 
		dxGiDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
		dxGiDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		//�ȼ��� ���� ���� ��, �̹��� ǰ�� ���� 0~1
		dxGiDesc.SampleDesc.Count = 1;
		dxGiDesc.SampleDesc.Quality = 0;

		//adapter = �����
		//GIdevice = ����Ÿ��
		//factory = ��üȭ��ó��
		Microsoft::WRL::ComPtr<IDXGIDevice> cpDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> cpDXGIAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> cpDXGIFactroy = nullptr;

		if (FAILED(m_cpDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)cpDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(cpDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)cpDXGIAdapter.GetAddressOf())))
			return false;

		if (FAILED(cpDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)cpDXGIFactroy.GetAddressOf())))
			return false;

		if (FAILED(cpDXGIFactroy->CreateSwapChain(m_cpDevice.Get(), &dxGiDesc, m_cpSwapChain.GetAddressOf())))
			return false;

		return true;
	}
	bool GraphicDevice_Dx11::CreateBuffer(ID3D11Buffer** _buffer, D3D11_BUFFER_DESC* _desc, D3D11_SUBRESOURCE_DATA* _data)
	{
		//D3D11_BUFFER_DESC triangleDesc = {};
		//triangleDesc.ByteWidth = desc->ByteWidth;
		//triangleDesc.BindFlags = desc->BindFlags;
		//triangleDesc.CPUAccessFlags = desc->CPUAccessFlags;


		/*D3D11_SUBRESOURCE_DATA triangleData = {};
		triangleData.pSysMem = vertexes;*/

		if(FAILED(m_cpDevice->CreateBuffer(_desc,_data,_buffer)))
			return false;

		return true;
	}
	

	bool GraphicDevice_Dx11::CompileFromfile(const std::wstring& _wstrFileName, const std::string& _strFunName, const std::string& _strVersion, ID3DBlob** _ppCod)
	{
		ID3DBlob* errorBlob = nullptr;
		D3DCompileFromFile(_wstrFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			_strFunName.c_str(), _strVersion.c_str(), 0, 0, _ppCod, &errorBlob);

		if (errorBlob)
		{
			OutputDebugStringA((char*)(errorBlob->GetBufferPointer()));
			errorBlob->Release();
			errorBlob = nullptr;
		}

		return true;
	}

	bool GraphicDevice_Dx11::CreateVertexShader(const void* _pShaderByteCode, 
		SIZE_T _BytecodeLength, 
		ID3D11VertexShader** _ppVertexShader)
	{
		if (FAILED(m_cpDevice->CreateVertexShader(_pShaderByteCode, _BytecodeLength, nullptr, _ppVertexShader)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateGeometryShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11GeometryShader** _ppGeometryShader)
	{
		if (FAILED(m_cpDevice->CreateGeometryShader(_pShaderBytecode, _BytecodeLength, nullptr, _ppGeometryShader)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreatePixelShader(const void* _pShaderByteCode, 
		SIZE_T _BytecodeLength, 
		ID3D11PixelShader** _ppPixelShader)
	{
		if (FAILED(m_cpDevice->CreatePixelShader(_pShaderByteCode, _BytecodeLength, nullptr, _ppPixelShader)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateComputeShader(const void* _pShaderByteCode, SIZE_T _BytecodeLength, ID3D11ComputeShader** _ppComputeShader)
	{
		if (FAILED(m_cpDevice->CreateComputeShader(_pShaderByteCode, _BytecodeLength, nullptr, _ppComputeShader)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateSampler(const D3D11_SAMPLER_DESC* _pSamplerDesc, ID3D11SamplerState** _ppSamplerState)
	{
		if(FAILED(m_cpDevice->CreateSamplerState(_pSamplerDesc,_ppSamplerState)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateRasterizeState(const D3D11_RASTERIZER_DESC* _pRasterizerDesc, ID3D11RasterizerState** _ppRasterizerState)
	{
		if(FAILED(m_cpDevice->CreateRasterizerState(_pRasterizerDesc,_ppRasterizerState)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* _pDepthStencilDesc, ID3D11DepthStencilState** _ppDepthStencilState)
	{
		if (FAILED(m_cpDevice->CreateDepthStencilState(_pDepthStencilDesc, _ppDepthStencilState)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateBlendState(const D3D11_BLEND_DESC* _pBlendStateDesc, ID3D11BlendState** _ppBlendState)
	{
		if (FAILED(m_cpDevice->CreateBlendState(_pBlendStateDesc, _ppBlendState)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateShaderResourceView(ID3D11Resource* _pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc, ID3D11ShaderResourceView** _ppSRVView)
	{
		if (FAILED(m_cpDevice->CreateShaderResourceView(_pResource,_pDesc,_ppSRVView)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateDepthStencilView(ID3D11Resource* _pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc, ID3D11DepthStencilView** _ppDepthStencilView)
	{
		if (FAILED(m_cpDevice->CreateDepthStencilView(_pResource, _pDesc, _ppDepthStencilView)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateRenderTargetView(ID3D11Resource* _pResource, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc, ID3D11RenderTargetView** _ppRTView)
	{
		if (FAILED(m_cpDevice->CreateRenderTargetView(_pResource, _pDesc, _ppRTView)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateUnorderedAccessView(ID3D11Resource* _pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* _pDesc, ID3D11UnorderedAccessView** _ppUAView)
	{
		if (FAILED(m_cpDevice->CreateUnorderedAccessView(_pResource, _pDesc, _ppUAView)))
			return false;

		return true;
	}
	
	bool GraphicDevice_Dx11::CreateTexture2D(const D3D11_TEXTURE2D_DESC* _desc, void* _pdata, ID3D11Texture2D** _ppTexture2D)
	{
		//2d �ؽ��� �ɼ�
		//D3D11_TEXTURE2D_DESC dxGiDesc = {};
		//dxGiDesc.BindFlags = _desc->BindFlags;
		//dxGiDesc.Usage = _desc->Usage;
		//dxGiDesc.CPUAccessFlags = 0;
		//
		//dxGiDesc.Format = _desc->Format;
		//dxGiDesc.Width = _desc->Width;
		//dxGiDesc.Height = _desc->Height;
		//dxGiDesc.ArraySize = _desc->ArraySize;
		//
		//dxGiDesc.SampleDesc.Count = _desc->SampleDesc.Count;
		//dxGiDesc.SampleDesc.Quality = 0;
		//
		//dxGiDesc.MipLevels = _desc->MipLevels;
		//dxGiDesc.MiscFlags = _desc->MiscFlags;

		//���̹���
		//if (FAILED(m_cpDevice->CreateTexture2D(&dxGiDesc, nullptr, m_cpDepthStencilBuffer.ReleaseAndGetAddressOf())))
		//	return false;
		//
		////���̹��� ��
		//if (FAILED(m_cpDevice->CreateDepthStencilView(m_cpDepthStencilBuffer.Get(), nullptr, m_cpDepthStencilView.GetAddressOf())))
		//	return false;

		if (FAILED(m_cpDevice->CreateTexture2D(_desc, nullptr, _ppTexture2D)))
			return false;

		return true;
	}
	bool GraphicDevice_Dx11::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* _pInputElementDescs, 
		UINT _iNumElements, 
		ID3DBlob* _byteCode, 
		ID3D11InputLayout** _ppInputLayout)
	{
		if (FAILED(m_cpDevice->CreateInputLayout(_pInputElementDescs, _iNumElements,
			_byteCode->GetBufferPointer(),
			_byteCode->GetBufferSize(),
			_ppInputLayout)))
			return false;

		return true;
	}
	void GraphicDevice_Dx11::BindViewPort(D3D11_VIEWPORT* _viewPort)
	{
		//����Ʈ ����
		m_cpContext->RSSetViewports(1, _viewPort);

	}
	void GraphicDevice_Dx11::BindRasterizeState(ID3D11RasterizerState* _pRasterizerState)
	{
		m_cpContext->RSSetState(_pRasterizerState);
	}
	void GraphicDevice_Dx11::BindDepthStencilState(ID3D11DepthStencilState* _pDepthStencilState)
	{
		m_cpContext->OMSetDepthStencilState(_pDepthStencilState,0);
	}
	void GraphicDevice_Dx11::BindBlendState(ID3D11BlendState* _pBlendState)
	{
		m_cpContext->OMSetBlendState(_pBlendState, nullptr, 0xffffffff);
	}
	void GraphicDevice_Dx11::CopyResource(ID3D11Resource* _pDstResource, ID3D11Resource* _pSrcResource)
	{
		m_cpContext->CopyResource(_pDstResource, _pSrcResource);
	}
	void GraphicDevice_Dx11::BindInputLayout(ID3D11InputLayout* _pInputLayout)
	{
		m_cpContext->IASetInputLayout(_pInputLayout);
	}
	void GraphicDevice_Dx11::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology)
	{
		m_cpContext->IASetPrimitiveTopology(_Topology);
	}
	void GraphicDevice_Dx11::BindVertexBuffer(UINT _iStartSlot,
		ID3D11Buffer* const* _ppVertexBuffers, 
		const UINT* _pStrides, 
		const UINT* _pOffsets)
	{
		m_cpContext->IASetVertexBuffers(_iStartSlot, 1, _ppVertexBuffers, _pStrides, _pOffsets);
	}

	void GraphicDevice_Dx11::BindIndexBuffer(ID3D11Buffer* pIndexBuffer, 
		DXGI_FORMAT _Format, 
		UINT _iOffset)
	{
		m_cpContext->IASetIndexBuffer(pIndexBuffer, _Format, _iOffset);
	}

	void GraphicDevice_Dx11::BindVertexShader(ID3D11VertexShader* _pVertexShader)
	{
		m_cpContext->VSSetShader(_pVertexShader,0,0);
	}

	void GraphicDevice_Dx11::BindHullShader(ID3D11HullShader* _pHullShader)
	{
		m_cpContext->HSSetShader(_pHullShader, 0, 0);
	}

	void GraphicDevice_Dx11::BindDomainShader(ID3D11DomainShader* _pDomainShader)
	{
		m_cpContext->DSSetShader(_pDomainShader, 0, 0);
	}

	void GraphicDevice_Dx11::BindGeometryShader(ID3D11GeometryShader* _pGeometryShader)
	{
		m_cpContext->GSSetShader(_pGeometryShader, 0, 0);
	}

	void GraphicDevice_Dx11::BindPixelShader(ID3D11PixelShader* _pPixelShader)
	{
		m_cpContext->PSSetShader(_pPixelShader, 0, 0);
	}

	void GraphicDevice_Dx11::BindComputeShader(ID3D11ComputeShader* _pComputeShader)
	{
		m_cpContext->CSSetShader(_pComputeShader, 0, 0);
	}

	void GraphicDevice_Dx11::Dispatch(UINT _iThreadGroupX, UINT _iThreadGroupY, UINT _iThreadGroupZ)
	{
		m_cpContext->Dispatch(_iThreadGroupX, _iThreadGroupY, _iThreadGroupZ);
	}

	void GraphicDevice_Dx11::SetConstantBuffer(ID3D11Buffer* _buffer, void* _data, UINT _iSize)
	{
		//cpu���� ���� ���� gpu�� ������ ���� ������ ���� ��������
		D3D11_MAPPED_SUBRESOURCE subRes = {};
		//���ҽ��� ��������� ���ε˴ϴ�. 
		m_cpContext->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
		//���� ����
		memcpy(subRes.pData, _data, _iSize);
		//cpu�� gpu���� ����
		m_cpContext->Unmap(_buffer, 0);//����� unmap
	}

	void GraphicDevice_Dx11::BindConstantBuffer(eShaderStage _eStage, eCBType _eType, ID3D11Buffer* _buffer)
	{
		switch (_eStage)
		{
		case eShaderStage::VS:
			//���� ���� ���� ���� 
			m_cpContext->VSSetConstantBuffers((UINT)_eType, 1, &_buffer);
			break;
		case eShaderStage::HS:
			m_cpContext->HSSetConstantBuffers((UINT)_eType, 1, &_buffer);
			break;
		case eShaderStage::DS:
			m_cpContext->DSSetConstantBuffers((UINT)_eType, 1, &_buffer);
			break;
		case eShaderStage::GS:
			m_cpContext->GSSetConstantBuffers((UINT)_eType, 1, &_buffer);
			break;
		case eShaderStage::PS:
			m_cpContext->PSSetConstantBuffers((UINT)_eType, 1, &_buffer);
			break;
		case eShaderStage::CS:
			m_cpContext->CSSetConstantBuffers((UINT)_eType, 1, &_buffer);
			break;
		case eShaderStage::END:
			break;
		default:
			break;
		}
	}
	void GraphicDevice_Dx11::BindBuffer(ID3D11Buffer* _pBuffer, void* _data, UINT _iSize)
	{
		D3D11_MAPPED_SUBRESOURCE sub = {};
		m_cpContext->Map(_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, _data, _iSize);
		m_cpContext->Unmap(_pBuffer, 0);
	}
	void GraphicDevice_Dx11::BindsConstantBuffer(eShaderStage _eStage, eCBType _eType, ID3D11Buffer* _buffer)
	{
		m_cpContext->VSSetConstantBuffers((UINT)_eType, 1, &_buffer);
		m_cpContext->HSSetConstantBuffers((UINT)_eType, 1, &_buffer);
		m_cpContext->DSSetConstantBuffers((UINT)_eType, 1, &_buffer);
		m_cpContext->GSSetConstantBuffers((UINT)_eType, 1, &_buffer);
		m_cpContext->PSSetConstantBuffers((UINT)_eType, 1, &_buffer);
		m_cpContext->CSSetConstantBuffers((UINT)_eType, 1, &_buffer);

	}
	void GraphicDevice_Dx11::BindShaderResource(eShaderStage _eStage, UINT _iStartSlot, ID3D11ShaderResourceView** _ppSRV)
	{
		switch (_eStage)
		{
		case eShaderStage::VS:
			m_cpContext->VSSetShaderResources(_iStartSlot, 1, _ppSRV);
			break;
		case eShaderStage::HS:
			m_cpContext->HSSetShaderResources(_iStartSlot, 1, _ppSRV);
			break;
		case eShaderStage::DS:
			m_cpContext->DSSetShaderResources(_iStartSlot, 1, _ppSRV);
			break;
		case eShaderStage::GS:
			m_cpContext->GSSetShaderResources(_iStartSlot, 1, _ppSRV);
			break;
		case eShaderStage::PS:
			m_cpContext->PSSetShaderResources(_iStartSlot, 1, _ppSRV);
			break;
		case eShaderStage::CS:
			m_cpContext->CSSetShaderResources(_iStartSlot, 1, _ppSRV);
			break;
		case eShaderStage::END:
			break;
		default:
			break;
		}
	}
	void GraphicDevice_Dx11::BinUnorderedAccess(UINT _iSolt, ID3D11UnorderedAccessView** _ppUnorderedAccessViews, const UINT* _pUAVInitialCounts)
	{
		m_cpContext->CSSetUnorderedAccessViews(_iSolt, 1, _ppUnorderedAccessViews,_pUAVInitialCounts);
	}
	void GraphicDevice_Dx11::BindSampler(eShaderStage _eStage, UINT _iStartSlot, ID3D11SamplerState** _ppSamplers)
	{
		switch (_eStage)
		{
		case eShaderStage::VS:
			m_cpContext->VSSetSamplers(_iStartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::HS:
			m_cpContext->HSSetSamplers(_iStartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::DS:
			m_cpContext->DSSetSamplers(_iStartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::GS:
			m_cpContext->GSSetSamplers(_iStartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::PS:
			m_cpContext->PSSetSamplers(_iStartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::CS:
			m_cpContext->CSSetSamplers(_iStartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::END:
			break;
		default:
			break;
		}
	}
	void GraphicDevice_Dx11::DrawIndexed(UINT _iIndexCount, UINT _iStartIndexLocation, INT _iBaseVertexLocation)
	{
		m_cpContext->DrawIndexed(_iIndexCount, _iStartIndexLocation, _iBaseVertexLocation);
	}
	void GraphicDevice_Dx11::DrawIndexedInstanced(UINT _iIndexCountPerInstance, UINT _iInstanceCount,
		UINT _iStartIndexLocation, INT _iBaseVertexLocation, UINT _iStartInstanceLocation)
	{
		m_cpContext->DrawIndexedInstanced(_iIndexCountPerInstance, _iInstanceCount, _iStartIndexLocation,
			_iBaseVertexLocation, _iStartInstanceLocation);
	}
	void GraphicDevice_Dx11::ClearTarget()
	{				
		//ȸ��
		FLOAT bgColor[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
		//����� �����
		m_cpContext->ClearRenderTargetView(m_cpRenderTarget->GetRTV().Get(), bgColor);
		//���̹��� �����
		m_cpContext->ClearDepthStencilView(m_cpDepthStencil->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
		//�ϳ� �̻��� ������ ����� ���ڼ����� ���ε��ϰ� ���� ���ٽ� ���۸� ���-���� �ܰ迡 ���ε��մϴ�.
		m_cpContext->OMSetRenderTargets(1, m_cpRenderTarget->GetRTV().GetAddressOf(), m_cpDepthStencil->GetDSV().Get());
	}
	void GraphicDevice_Dx11::UpdateViewPort()
	{
		//����Ʈ ������Ʈ
		HWND hWnd = application.GetHwnd();

		//�ӽ�
		//float fHeight = application.GetHeight();
		//float fWidth = application.GetWidth();

		RECT winRect = {};
		GetClientRect(hWnd, &winRect);

		//float fDiffY = abs(fHeight - (float)(winRect.bottom - winRect.top));
		//float fDiffX = abs(fWidth - (winRect.right - winRect.left));

		m_tViewPort =
		{
			0.0f, 0.0f
			, (float)(winRect.right - winRect.left)// + fDiffX
			//,(float)750.f
			, (float)(winRect.bottom - winRect.top)// + fDiffY
			, 0.0f, 1.0f
		};
		BindViewPort(&m_tViewPort);
		
	}
	void GraphicDevice_Dx11::Draw()
	{	
		
		// input assembler ���� ������ ����
		//UINT vertexsize = sizeof(renderer::Vertex);
		//UINT offset = 0;
		//������ ���� �迭�� �Է� ����� �ܰ迡 ���ε��մϴ�.
		//m_cpContext->IASetVertexBuffers(0, 1, &renderer::triangleBuffer, &vertexsize, &offset);

		//UINT����Ʈ�ε���
		//m_cpContext->IASetIndexBuffer(renderer::triangleIdxBuffer, DXGI_FORMAT_R32_UINT, 0);
		// 
		//�۹� ���ε�
		//renderer::mesh->BindBuffer();
		
		//m_cpContext->IASetInputLayout(renderer::shader->GetInputLayout());
		//��������
		//W::graphics::GetDevice()->BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//��� �׸���
		//m_cpContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//bind vs, ps
		//m_cpContext->VSSetShader(renderer::triangleVSShader, 0, 0);
		//m_cpContext->PSSetShader(renderer::trianglePSShader, 0, 0);

		//renderer::shader->Binds();

		//m_cpContext->Draw(3, 0);
		// ps���� ���� �������� �����͵��� �����Ǳ� ������ �ȼ��� �߰����� ����
		//�����Ͷ��������� �޸����� ������ ����� �յΤӸ� �ǰ��ϱ� ������ ������ �����(���ֺ��������� �޸�) ���� �����°� �޶���
		//m_cpContext->DrawIndexed(renderer::mesh->GetIndexCount(), 0, 0);

		//����Ÿ�ٿ� �ִ� �̹����� ȭ�鿡 �׷��ش�
		//m_cpSwapChain->Present(0, 0);
	}

	void GraphicDevice_Dx11::Present()
	{
		m_cpSwapChain->Present(0, 0);
	}


	

}
