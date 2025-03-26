#include "WMesh.h"
#include "WRenderer.h"
//mesh�� �׹�ó�� ���̾��� �������� �������� index�� vertex ���۷� ����
//mesh�� cpu���� load �� gpu�� ������ ���̴����� ó��
//���� ���������͸� ��� ó�������� ���̴���ǥ�� ���̴�, �Ž� ���� ���ҽ��� ����
//���̴�, ���, 
// gpu�� �̹������� �ٸ� vb ib�� �ֱ� ������ ���������ο� �������༭ �ٲ���Ѵ�
// ������ �ٲٰ� �׸��� ���޾� ���������ο��� ó��
// ���ۼ������� �� shader���� �� ���������ο� ����

namespace W
{
    Mesh::Mesh() :
        Resource(enums::eResourceType::Mesh),
        m_cpVertexBuffer(nullptr),
        m_cpIndexBuffer(nullptr),
        m_tVBDesc{},
        m_tIBDesc{},
        m_iIndexCount(0)
    {
        
    }

    Mesh::~Mesh()
    {
        
    }

    HRESULT Mesh::Load(const std::wstring& path)
    {
        return E_NOTIMPL;
    }

    bool Mesh::CreateVertexBuffer(void* _data, UINT _iCount)
    {
        m_tVBDesc.ByteWidth = sizeof(renderer::Vertex) * _iCount;
        m_tVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
        m_tVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;//GPU���� �б� �� ���� �׼����� �ʿ��� ���ҽ��Դϴ�.
        m_tVBDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sub = {};//���� ���ҽ�
        sub.pSysMem = _data;
        
        //�� ������ �ּ��� �ּҰ�
        if(!GetDevice()->CreateBuffer(m_cpVertexBuffer.GetAddressOf(),&m_tVBDesc,&sub))
            return false;

        return true;
    }

    bool Mesh::CreateIndexBuffer(void* _data, UINT _iCount)
    {
        m_iIndexCount = _iCount;
        m_tIBDesc.ByteWidth = sizeof(UINT) * _iCount;
        m_tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
        m_tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        m_tIBDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sub = {};
        sub.pSysMem = _data;

        if (!GetDevice()->CreateBuffer(m_cpIndexBuffer.GetAddressOf(), &m_tIBDesc, &sub))
            return false;

        return true;
    }

    void Mesh::BindBuffer()
    {
        //�ϳ��� ������ ������ ũ��
        UINT iStride = sizeof(renderer::Vertex);
        //���� ���� ��ġ
        UINT iOffset = 0;

        //���� ����
        GetDevice()->BindVertexBuffer(0, m_cpVertexBuffer.GetAddressOf(), &iStride, &iOffset);
        GetDevice()->BindIndexBuffer(m_cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }
    void Mesh::Render()
    {
        GetDevice()->DrawIndexed(m_iIndexCount, 0, 0);
    }
    void Mesh::RenderInstanced(UINT _iStartindexLocation)
    {
        GetDevice()->DrawIndexedInstanced(m_iIndexCount, _iStartindexLocation,0,0,0);
    }
}

