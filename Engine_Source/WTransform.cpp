#include "WTransform.h"
#include "WSceneManger.h"
#include "Room.h"
#include "Transform.pb.h"
#include "ClientPacketHandler.h"

namespace W
{

	Transform::Transform():
		Component(eComponentType::Transform),
		m_vPosition(Vector3::Zero),
		m_vPrevPosition(Vector3::Zero),
		m_vRotation(Vector3::Zero),
		m_vScale(Vector3::One),
		m_pParentTransform(nullptr)
	{

	}
	Transform::Transform(const Transform& _pOrigin):
		Component(eComponentType::Transform),
		m_vPosition(_pOrigin.m_vPosition),
		m_vRotation(_pOrigin.m_vRotation),
		m_vScale(_pOrigin.m_vScale),
		m_pParentTransform(nullptr)
	{
	}
	Transform::~Transform()
	{

	}
	Transform* Transform::CreateClone()
	{
		return new Transform(*this);
	}
	void Transform::Initialize()
	{

	}
	void Transform::Update()
	{

	}
	void Transform::LateUpdate()
	{
		m_vWorld = Matrix::Identity;//전지 행렬

		//크기 행렬 생성
		Matrix mScale = Matrix::CreateScale(m_vScale);

		Matrix mRotation;
		mRotation = Matrix::CreateRotationX(m_vRotation.x);
		mRotation = Matrix::CreateRotationY(m_vRotation.y);
		mRotation = Matrix::CreateRotationZ(m_vRotation.z);

		//getOBB_World
		Matrix mPosition;
		mPosition.Translation(m_vPosition);

		m_vWorld = mScale * mRotation * mPosition;
		
		m_vUp = Vector3::TransformNormal(Vector3::Up, mRotation);
		m_vFoward = Vector3::TransformNormal(Vector3::Forward, mRotation);
		m_vRight = Vector3::TransformNormal(Vector3::Right, mRotation);

		//내 로컬 -> 월드 -> 부모 행렬
		if (m_pParentTransform)
			m_vWorld *= m_pParentTransform->m_vWorld;

	}

	void Transform::Render()
	{

	}

	void Transform::SendTransform()
	{
		if (m_vPosition == m_vPrevPosition)
			return;

		std::vector<UINT> vecID = SceneManger::GetPlayerIDs(GetOwner()->GetSceneName());

		Protocol::S_TRANSFORM pkt;
		pkt.set_x(m_vPosition.x);	pkt.set_y(m_vPosition.y);	pkt.set_z(m_vPosition.z);
		UCHAR cLayer = (UINT)GetOwner()->GetLayerType();
		UINT iObjectID = GetOwner()->GetObjectID();

		pkt.set_layer_id((cLayer<<24) | iObjectID);
	
		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GRoom.Unicast(pSendBuffer, vecID);
	}

}