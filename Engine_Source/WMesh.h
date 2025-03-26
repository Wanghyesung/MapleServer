#pragma once

#include "WResource.h"
#include "WGraphicDevice_Dx11.h"


namespace W
{
	using namespace graphics;
	class Mesh : public Resource
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual HRESULT Load(const std::wstring& path) override;
	
		bool CreateVertexBuffer(void* _data, UINT _iCount);
		bool CreateIndexBuffer(void* _data, UINT _iCount);

		void BindBuffer(); //buffer ���̺���ο� �����ش�.
		void Render();
		void RenderInstanced(UINT _iStartindexLocation);

		UINT GetIndexCount() { return m_iIndexCount; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cpVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cpIndexBuffer;

		D3D11_BUFFER_DESC m_tVBDesc;
		D3D11_BUFFER_DESC m_tIBDesc;

		UINT m_iIndexCount;
	};
}


