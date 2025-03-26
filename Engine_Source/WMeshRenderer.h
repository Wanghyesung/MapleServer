#pragma once

#include "WComponent.h"
#include "WMesh.h"
//#include "WShader.h"
#include "WMaterial.h"

namespace W
{
	//���� ������Ʈ������ mesh shader�� ���ؼ� �׷��� ex : mesh renderer
	//���׸���((shader)vs, ps, texture)
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		MeshRenderer(const MeshRenderer& _pOrigin);
		virtual ~MeshRenderer();


		virtual MeshRenderer* CreateClone()override;

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetMesh(std::shared_ptr<Mesh> _pMesh) { m_pMesh = _pMesh; }
		void SetMaterial(std::shared_ptr<Material> _pMaterial) { m_pMaterial = _pMaterial; }
		std::shared_ptr<Material> GetMaterial() { return m_pMaterial;}
		std::shared_ptr<Mesh> GetMesh() { return m_pMesh; }

	private:
		std::shared_ptr<Mesh> m_pMesh;
		//Shader* m_pShader;
		std::shared_ptr<Material> m_pMaterial;
	};
}

