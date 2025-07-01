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
		if (m_vPosition == m_vPrevPosition &&
			m_vRotation == m_vPrevRotation)
			return;	

		Protocol::S_TRANSFORM pkt;

		Protocol::TransformInfo* tTrInfo = pkt.mutable_transform();
		tTrInfo->set_p_x(m_vPosition.x);	tTrInfo->set_p_y(m_vPosition.y);	tTrInfo->set_p_z(m_vPosition.z);
		tTrInfo->set_r_x(m_vRotation.x);	tTrInfo->set_r_y(m_vRotation.y);	tTrInfo->set_r_z(m_vRotation.z);

		GameObject* pOwner = GetOwner();
		UCHAR cLayer = (UINT)pOwner->GetLayerType();
		UINT iObjectID = pOwner->GetObjectID();

		pkt.set_layer_id((cLayer<<24) | iObjectID);
	
		shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		const vector<UINT>& vecTarget = pOwner->GetExclusiveClients();
		if (vecTarget.empty())
			GRoom.Unicast(pSendBuffer, SceneManger::GetPlayerIDs(pOwner->GetSceneName()));
		else
			GRoom.Unicast(pSendBuffer, vecTarget);
	}

}