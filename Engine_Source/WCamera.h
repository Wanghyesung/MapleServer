#pragma once
#include "WComponent.h"
#include "WGraphics.h"

//UIī�޶�(�̵�X), ĳ���� ī�޶� ����
//renderer render�Լ����� ī�޶� ����ȣ��
//Scene, camerascript engine������ �̵�
//opaque ������ cutout �κ� ������ transparnt���� ������ ���� ������ ���� ������� �������
//��������������� ������ ����Ÿ�� ������� ����
//���̾�� ī�޶� ���� ���밡�� 
//SceneMgr -> camera(render)�� ����������� ������


//������ ���� ������ ���ص� ���̹��ۿ��� �Ȱ��� z������ ���� �׷��� 
// �긦 �׸��� ������ �� �ڸ��� �� ���� z���� ��ü�� �׸��� ����
//z���� �ؼ� �׸��� ������ ������Ԥ�
namespace W
{
	class Camera : public Component
	{
	public:
		enum class eProjectionType
		{
			Perpectivem,
			OrthoGraphic,
			None,
		};
		

		Camera();
		virtual ~Camera();

		static Matrix& GetGpuViewMatrix() { return View; }
		static Matrix& GetGpuProjectionMatrix() { return Projection; }
		static void SetGpuViewMatrix(Matrix _View) { View = _View; }
		static void SetGpuProjectionMatrix(Matrix _Projection) { Projection = _Projection; }

		bool CreateViewMatrix();
		bool CreateProjectionMatrix(eProjectionType _eType);
		void RegisterCameraInRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;


		void TurnLayerMask(eLayerType _eType, bool _bEnable = true);
		void TurnUILayerMask();

		void EnableLayerMasks() { m_bitLayerMask.set(); }
		void DisableLayerMasks() { m_bitLayerMask.reset(); }

		void AlphaSortGameObjects();
		void ZSortTransparencyGameObjects();
		void DivideAlphaBlendGameObjects(const std::vector<GameObject*> _vecGameObj);
		void RenderOpaque();
		void RenderCutOut();
		void RenderTransparent();

		void EnableDepthStencilState();
		void DisablDepthStencilState();

		float GetSize() { return m_fSize; }
		Matrix& GetViewMatrix() { return m_mView; }
		Matrix& GetProjectionMatrix() { return m_mProjection; }

	private:
		//��������� ������Ʈ���� �ٸ�����
		//�� ������ ��������� ��� ������Ʈ�鿡�� �Ȱ���
		//����Ǳ� ������ static����
		Matrix m_mView;
		Matrix m_mProjection;

		static Matrix View;
		static Matrix Projection;

		eProjectionType m_eType;
		float m_fAspectRatio;//��Ⱦ��
		float m_fNear;
		float m_fFal;
		float m_fSize;
	

		//�׸��� ������� ������Ʈ ����
		std::bitset<(UINT)eLayerType::End> m_bitLayerMask;
		std::vector<GameObject*> m_vecOpaqueGameObjects;
		std::vector<GameObject*> m_vecCutOutGameObjects;
		std::vector<GameObject*> m_vecTransparentGameObjects;
 	};
}


