#include "WMeshRenderer.h"
#include "WGameObject.h"
#include "WTransform.h"
#include "WRenderer.h"
#include "WAnimator.h"
namespace W
{
	MeshRenderer::MeshRenderer():
		Component(eComponentType::MeshRenderer)
	{
	}
	MeshRenderer::MeshRenderer(const MeshRenderer& _pOrigin):
		Component(eComponentType::MeshRenderer),
		m_pMaterial(_pOrigin.m_pMaterial),
		m_pMesh(_pOrigin.m_pMesh)
	{

	}
	MeshRenderer::~MeshRenderer()
	{

	}
	MeshRenderer* MeshRenderer::CreateClone()
	{
		return new MeshRenderer(*this);
	}
	void MeshRenderer::Initialize()
	{

	}
	void MeshRenderer::Update()
	{

	}
	void MeshRenderer::LateUpdate()
	{
	}
	void MeshRenderer::Render()
	{
		//lateupdat�� update���� ������ ���� mesh, shader���� ��ġ���� ���ļ� ����
		Transform* pTransform = GetOwner()->GetComponent<Transform>();
		pTransform->BindConstantBuffer();

		//mMesh->BindBuffer();
		//mShader->Binds();

		m_pMesh->BindBuffer();
		m_pMaterial->Binds();

		Animator* pAnimation = GetOwner()->GetComponent<Animator>();
		if (pAnimation)
			pAnimation->Binds();

		m_pMesh->Render();

		//�ؽ��Ŀ� �����ְ� �׸� �� �ؽ��� �����
		m_pMaterial->Clear();
		//GetDevice()->DrawIndexed(renderer::mesh->GetIndexCount(), 0, 0);
	}
}