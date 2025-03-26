#include "guiEditor.h"
#include "..\Engine\WGridScript.h"
#include "..\Engine_Source\WMesh.h"
#include "..\Engine_Source\WResources.h"
#include "..\Engine_Source\WTransform.h"
#include "..\Engine_Source\WMeshRenderer.h"
#include "..\Engine_Source\WMaterial.h"
#include "..\Engine_Source\WRenderer.h"


namespace gui
{
	using namespace W::enums;
	std::vector<Widget*> Editor::m_vecWidgets = {};
	std::vector<EditorObject*> Editor::m_vecEditorObjects = {};
	std::vector<DebugObject*> Editor::m_vecDebugOjbects = {};


	void Editor::Initialize()
	{
		m_vecDebugOjbects.resize((UINT)eColliderType::End);
		
		std::shared_ptr<W::Mesh> mesh
			= W::Resources::Find<W::Mesh>(L"DebugRect");
		std::shared_ptr<W::Material> material
			= W::Resources::Find<W::Material>(L"DebugMaterial");
		
		m_vecDebugOjbects[(UINT)eColliderType::Rect] = new DebugObject();
		m_vecDebugOjbects[(UINT)eColliderType::Rect]->AddComponent<W::Transform>();
		W::MeshRenderer* mr
			= m_vecDebugOjbects[(UINT)eColliderType::Rect]->AddComponent<W::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);
		
		
		//EditorObject* grid = new EditorObject();
		//grid->SetName(L"Grid");
		//
		//mr = grid->AddComponent<W::MeshRenderer>();
		//mr->SetMesh(W::Resources::Find<W::Mesh>(L"RectMesh"));
		//mr->SetMaterial(W::Resources::Find<W::Material>(L"GridMaterial"));
		//W::GridScript* gridSc = grid->AddComponent<W::GridScript>();
		//gridSc->SetCamera(renderer::m_vecCameras[0]);//����ī�޶� 0�� �ε���
		//
		//m_vecEditorObjects.push_back(grid);

	}

	void Editor::Run()
	{
		Update();
		LateUpdate();
		Render();
	}

	void Editor::Update()
	{
		for (EditorObject* obj : m_vecEditorObjects)
		{
			obj->Update();
		}
	}

	void Editor::LateUpdate()
	{
		for (EditorObject* obj : m_vecEditorObjects)
		{
			obj->LateUpdate();
		}
	}

	void Editor::Render()
	{
		for (EditorObject* obj : m_vecEditorObjects)
		{
			obj->Render();
		}

		for (const W::graphics::DebugMesh& mesh
			: renderer::m_vecDebugMeshs)
		{
			DebugRender(mesh);
		}
		renderer::m_vecDebugMeshs.clear();
	}

	void Editor::Release()
	{
		for (auto widget : m_vecWidgets)
		{
			delete widget;
			widget = nullptr;
		}

		for (auto editorObj : m_vecEditorObjects)
		{
			delete editorObj;
			editorObj = nullptr;
		}

		for (auto debugObj : m_vecDebugOjbects)
		{
			delete debugObj;
			debugObj = nullptr;
		}

	}

	void Editor::DebugRender(const W::graphics::DebugMesh& _mesh)
	{
		//collider�� ���� ��ü�� ��ġ�� ũ�� ȸ���� �����ͼ� �� ��ü ���� �׸�
		//�̸� ������ �簢�� �� �޽����� ����� ������Ʈ�� ���
		if (m_vecDebugOjbects.size() == 0)
			return;

		if (_mesh.active == false)
			return;

		DebugObject* debugObj = m_vecDebugOjbects[(UINT)_mesh.type];
		
		// ��ġ ũ�� ȸ�� ������ �޾ƿͼ�
		// �ش� ���ӿ�����Ʈ���� �׷��ָ�ȴ�.
		W::Transform* debugTr = debugObj->GetComponent<W::Transform>();

		//�޽� ������ ������ �������� ����� ������Ʈ ������� ���ε�
		Vector3 vPos = _mesh.position;
		vPos.z -= 0.01f;

		debugTr->SetPosition(vPos);
		debugTr->SetScale(_mesh.scale);
		debugTr->SetRotation(_mesh.rotation);

		//��� ���
		debugTr->LateUpdate();

		W::Camera* pMainCamera = renderer::MainCamera;

		//renderer���� render�ϰ� ī�޶� ���� ����� ������ ����ī�޶�
		//�����ͼ� ����ī�޶�� ����

		W::Camera::SetGpuViewMatrix(pMainCamera->GetViewMatrix());
		W::Camera::SetGpuProjectionMatrix(pMainCamera->GetProjectionMatrix());
		
		debugObj->Render();
	}

}