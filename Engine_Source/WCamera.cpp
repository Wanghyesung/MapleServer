#include "WCamera.h"
#include "WTransform.h"
#include "WGameObject.h"
#include "WApplication.h"
#include "WRenderer.h"
#include "WScene.h"
#include "WSceneManger.h"
#include "WMeshRenderer.h"
//�� �ɰ���

//���׸��� �ٲ𶧸��� �ش� �ؽ���(shader�� ���ε���) �ʱ�ȭ 
//���������� ���׸���, �޽� �׸��� clear

//orthographic 2���� ũ�� �Ȱ���
//perpective �ڷ� ������ �۾�����
//near ����ü ��(�պκ� ©�� ��밡��)
//far ����ü ��(�þ�)
//ī�޶�� �̵�, ȸ����ĸ�
//������Ʈ�� ī�޶��� �̵��ݴ�������� -pos
//ī�޶��� �������͸� Ȱ���ؼ� ȸ��(���Ϸ���X)
//createlookat�Լ��� ���� ������ ���ڿ� ���ؼ� ����
//���忡���� srt������ �ٲپ�������� view�������� tr��������̾��� ��������� ȸ���κ������ ��ħ
//����������� ��Ⱦ�����ؾ��� ����/ ����
//LH�޼���ǥ�� �����ΰ����� -
//perpective�� fov �þ߰�(����) �߰�
//��������� ������۷� shade��
//ī�޶� �̵��� ��ü �ݴ�������

extern W::Application application;

namespace W
{
	bool CompareZSort(GameObject* a, GameObject* b)
	{
		if (a->GetComponent<Transform>()->GetPosition().z
			<= b->GetComponent<Transform>()->GetPosition().z)
			return false;

		return true;
	}

	Matrix Camera::View = Matrix::Identity;
	Matrix Camera::Projection = Matrix::Identity;

	Camera::Camera():
		Component(eComponentType::Camera),
		m_eType(eProjectionType::OrthoGraphic),
		m_fAspectRatio(1.f),
		m_fNear(1.f),
		m_fFal(1000.f),
		m_fSize(10.f),
		m_bitLayerMask{},
		m_vecOpaqueGameObjects{},
		m_vecCutOutGameObjects{},
		m_vecTransparentGameObjects{},
		m_mView{},
		m_mProjection{}
	{
		//���̾� ����ũ ���� true�� �ʱ�ȭ
		EnableLayerMasks();
	}
	Camera::~Camera()
	{
		
	}
	void Camera::Initialize()
	{
		
	}
	void Camera::Update()
	{	

	}
	void Camera::LateUpdate()
	{
		//lateupdate���� �� ī�޶��� �����, ������� ���ϱ�
		CreateViewMatrix();
		CreateProjectionMatrix(m_eType);

		//������Ʈ���� ������Ʈ�� ī�޶��߰��ϰ� �׸���
		RegisterCameraInRenderer();
	}
	void Camera::Render()
	{
		//������ ī�޶󸶴� �ٸ� ��,�������
		View = m_mView;
		Projection = m_mProjection;

		AlphaSortGameObjects();
		ZSortTransparencyGameObjects();

		RenderOpaque();

		//������ ���� �׸��� ���� z���� ������ �׸��� z�� �켱�������� �з��� �ȱ׷���
		//z���� ����
		DisablDepthStencilState();
		//������ -> ������ -> ����
		RenderCutOut();
		RenderTransparent();
		EnableDepthStencilState();
	}
	
	void Camera::RegisterCameraInRenderer()
	{
		renderer::m_vecCameras.push_back(this);
	}

	void Camera::TurnLayerMask(eLayerType _eType, bool _bEnable)
	{
		m_bitLayerMask.set((UINT)_eType, _bEnable);
	}

	void Camera::TurnUILayerMask()
	{
		for (UINT i = 0; i < (UINT)eLayerType::End; ++i)
		{
			if (i == (UINT)eLayerType::UI)
				continue;

			m_bitLayerMask.set(i, false);
		}

	}

	void Camera::AlphaSortGameObjects()
	{
		// ������ �Ǳ� ���� �� ����� �ٽ� ����
		//������ ���� �ڿ� ������
		m_vecOpaqueGameObjects.clear();
		m_vecCutOutGameObjects.clear();
		m_vecTransparentGameObjects.clear();

		Scene* pScene = SceneManger::GetActiveScene();
		for (size_t i = 0; i < (UINT)eLayerType::End; i++)
		{
			//���̾��ũ true�� ������Ʈ�� ������
			if (m_bitLayerMask[i] == true)
			{
				Layer& layer = pScene->GetLayer((eLayerType)i);
				const std::vector<GameObject*> vecGameObjects =
					layer.GetGameObjects();

				DivideAlphaBlendGameObjects(vecGameObjects);
			}
		}
	}

	void Camera::ZSortTransparencyGameObjects()
	{
		std::sort(m_vecCutOutGameObjects.begin(),
			m_vecCutOutGameObjects.end(),
			CompareZSort);

		std::sort(m_vecTransparentGameObjects.begin(),
			m_vecTransparentGameObjects.end(),
			CompareZSort);

	}
	
	void Camera::DivideAlphaBlendGameObjects(const std::vector<GameObject*> _vecGameObj)
	{
		for (GameObject* obj : _vecGameObj)
		{
			MeshRenderer* pMeshRender =
				obj->GetComponent<MeshRenderer>();

			if (pMeshRender == nullptr)
				continue;

			std::shared_ptr<Material> spMater = pMeshRender->GetMaterial();
			eRenderingMode eMode = spMater->GetRenderinMode();

			switch (eMode)
			{
			case eRenderingMode::Opaque:
				m_vecOpaqueGameObjects.push_back(obj);
				break;
			case eRenderingMode::CutOut:
				m_vecCutOutGameObjects.push_back(obj);
				break;
			case eRenderingMode::Transparent:
				m_vecTransparentGameObjects.push_back(obj);
				break;
			default:
				break;
			}
		}
	}


	void Camera::RenderOpaque()
	{
		for (GameObject* gameObj : m_vecOpaqueGameObjects)
		{
			if (gameObj == nullptr)
				continue;
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	void Camera::RenderCutOut()
	{
		for (GameObject* gameObj : m_vecCutOutGameObjects)
		{
			if (gameObj == nullptr)
				continue;
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	void Camera::RenderTransparent()
	{
		for (GameObject* gameObj : m_vecTransparentGameObjects)
		{
			if (gameObj == nullptr)
				continue;
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	bool Camera::CreateViewMatrix()
	{
		Transform* pTr = GetOwner()->GetComponent<Transform>();
		Vector3 vPos = pTr->GetPosition();

		m_mView = Matrix::Identity;
		m_mView *= Matrix::CreateTranslation(-vPos);//ī�޶��� �ݴ�� ��ü�� �̵�
		
		Vector3 up = pTr->Up();
		Vector3 right = pTr->Right();
		Vector3 foward = pTr->Foward();

		//.�� Ȱ���Ͽ� ��� �����Ϳ� ����
		//ī�޶��� �������͸� Ȱ���ؼ� ȸ��(���Ϸ���X)
		//createlookat�Լ��� ���� ������ ���ڿ� ���ؼ� ����
		Matrix mViewRotate;
		mViewRotate._11 = right.x;	mViewRotate._12 = up.x;	mViewRotate._13 = foward.x;
		mViewRotate._21 = right.y;	mViewRotate._22 = up.y;	mViewRotate._23 = foward.y;
		mViewRotate._31 = right.z;	mViewRotate._32 = up.z;	mViewRotate._33 = foward.z;
		m_mView *= mViewRotate;

		return true;
	}
	bool Camera::CreateProjectionMatrix(eProjectionType _eType)
	{
		RECT tRect = {};
		GetClientRect(application.GetHwnd(), &tRect);
		float fWidth = tRect.right - tRect.left; //1400
		float fHeight = tRect.bottom - tRect.top; //750
		m_fAspectRatio = fWidth / fHeight; //���� / ����

		if (_eType == eProjectionType::OrthoGraphic)
		{
			//orthorgraphic����
			float fOrthorGraphicRatio = m_fSize / 1000.f;
			fWidth *= fOrthorGraphicRatio;
			fHeight *= fOrthorGraphicRatio;

			//�޼���ǥ�� �������
			m_mProjection = Matrix::CreateOrthographicLH(fWidth, fHeight, m_fNear, m_fFal);
		}
		else
		{
			m_mProjection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.f, m_fAspectRatio, m_fNear, m_fFal);
		}


		return true;
	}
	
	void Camera::EnableDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::m_cpDepthStencilStates[(UINT)eDSType::Less];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}

	void Camera::DisablDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::m_cpDepthStencilStates[(UINT)eDSType::None];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}
	
}
