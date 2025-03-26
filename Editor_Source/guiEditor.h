#pragma once

#include "..\Engine_Source\Common.h";
#include "..\Engine_Source\Engine.h"
#include "guiWidget.h"
#include "guiEditorObject.h"
#include "guiDebugObject.h"
#include "..\Engine_Source\WGraphics.h"

//colldier �� onwer ������ ����׸޽��� �����ϰ� ����� ������Ʈ���� �ݶ��̴��� �׸�
//����׿�����Ʈ�� UIī�޶� ��ĸ� �޾Ƽ� ���� ������
//0�� mainī�޶�� �����ؼ� 0�� ����ī�޶��� �� ���� ��ķ� �׸�
// �� �ݶ��̴� �����

namespace gui
{
	class Editor
	{
	public:
		static void Initialize();
		static void Run();

		static void Update();
		static void LateUpdate();
		static void Render();
		static void Release();

		static void DebugRender(const W::graphics::DebugMesh& _mesh);

	private:
		static std::vector<Widget*> m_vecWidgets;
		static std::vector<EditorObject*> m_vecEditorObjects;
		static std::vector<DebugObject*> m_vecDebugOjbects;
	};
}


