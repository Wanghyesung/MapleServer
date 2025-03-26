#pragma once

#include "WComponent.h"

namespace W
{
	//Ʈ�������� ��ġ������ ����->������ۿ� ���޿�
	//	��ġ ������Ʈ�� ���ϸ��� ȣ��
	//	��ġ ������Ʈ(������ۼ���(ȣ��� ��ġ���õ�, �������� ������� ���� vector4(*3)))

	class Transform : public Component
	{
	public:
		Transform();
		Transform(const Transform& _pOrigin);
		virtual ~Transform();

		virtual Transform* CreateClone()override;

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void BindConstantBuffer();

		void SetPosition(Vector3 _vPosition) { m_vPosition = _vPosition; }
		void SetRotation(Vector3 _vRotation) { m_vRotation = _vRotation; }
		void SetScale(Vector3 _vScale) { m_vScale = _vScale; }

		void SetPosition(float x, float y, float z) { m_vPosition = Vector3(x, y, z); }
		void SetRotation(float x, float y, float z) { m_vRotation = Vector3(x, y, z); }
		void SetScale(float x, float y, float z) { m_vScale = Vector3(x, y, z); }

		Vector3 GetPosition() { return m_vPosition; }
		Vector3 GetRotation() { return m_vRotation; }
		Vector3 GetScale() { return m_vScale; }

		Vector3 Foward() { return m_vFoward; }
		Vector3 Right() { return m_vRight; }
		Vector3 Up() { return m_vUp; }

		void SetParent(Transform* transform) { m_pParentTransform = transform; }
		Transform* GetParent() { return m_pParentTransform; }
		Matrix& GetMatrix() { return m_vWorld; }
	private:
		Vector3 m_vPosition;
		Vector3 m_vRotation;
		Vector3 m_vScale;

		//�� ������ ����
		Vector3 m_vUp;
		Vector3 m_vFoward;
		Vector3 m_vRight;

		//�� ������Ʈ ���� ���
		Matrix m_vWorld;

		Transform* m_pParentTransform;
	};

}

