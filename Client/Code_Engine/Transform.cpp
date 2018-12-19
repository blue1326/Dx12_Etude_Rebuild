#include "Transform.h"

CTransform::CTransform(const shared_ptr<DxDevice> _device)
	:m_DxDevice(_device)
	,m_pParentMatrix(nullptr)
	,m_MatWorld(XMMatrixIdentity())
	,m_MatWorld_NonRot(XMMatrixIdentity())
{
	
	ZeroMemory(m_vInfomation, sizeof(XMVECTOR)*INFO_END);
}

CTransform::CTransform(const CTransform& rhs)
	:m_DxDevice(rhs.m_DxDevice)
	, m_pParentMatrix(rhs.m_pParentMatrix)
	, m_MatWorld(rhs.m_MatWorld)
	, m_MatWorld_NonRot(rhs.m_MatWorld_NonRot)
{
	memcpy(m_vInfomation, rhs.m_vInfomation, sizeof(XMVECTOR)*INFO_END);
}

CTransform::~CTransform()
{

}

HRESULT CTransform::Init_Component()
{
	//ZeroMemory(m_vInfomation, sizeof(XMVECTOR)*INFO_END);
	m_vInfomation[INFO_SCALE] = XMVectorSet(1, 1, 1,0);//XMVECTOR(1.f, 1.f, 1.f);
	m_vInfomation[INFO_POSITION] = XMVectorSet(0, 0, 0, 0);
	m_vInfomation[INFO_ANGLE] = XMVectorSet(0, 0, 0, 0);
	return S_OK;
}

void CTransform::Update_Component(const std::shared_ptr<CTimer> t)
{
	XMMATRIX matScale, matRotX, matRotY, matRotZ, matTrans;

	
	matScale =  XMMatrixScaling(XMVectorGetX(m_vInfomation[INFO_SCALE]),
								XMVectorGetY(m_vInfomation[INFO_SCALE]),
								XMVectorGetZ(m_vInfomation[INFO_SCALE]));
	matRotX = XMMatrixRotationX(XMVectorGetX(m_vInfomation[INFO_ANGLE]));
	matRotY = XMMatrixRotationY(XMVectorGetY(m_vInfomation[INFO_ANGLE]));
	matRotZ = XMMatrixRotationZ(XMVectorGetZ(m_vInfomation[INFO_ANGLE]));
	matTrans = XMMatrixTranslation(XMVectorGetX(m_vInfomation[INFO_POSITION]),
									XMVectorGetY(m_vInfomation[INFO_POSITION]),
									XMVectorGetZ(m_vInfomation[INFO_POSITION]));

	m_MatWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
	m_MatWorld_NonRot = matScale * matTrans;
	if (nullptr != m_pParentMatrix)
	{
		m_MatWorld *= *m_pParentMatrix;
		m_MatWorld_NonRot *= *m_pParentMatrix;
	}
}

std::shared_ptr<CComponent> CTransform::Clone()
{
	std::shared_ptr<CComponent> instance = std::shared_ptr<CComponent>(new CTransform(*this));
	//CopyComponentSettings(instance);
	return instance;
}


DirectX::XMMATRIX CTransform::GetWorld()
{
	return m_MatWorld;// make_shared(m_matWorld);
}

float CTransform::GetAngle(ANGLE eType)
{
	switch (eType)
	{
	case ANGLE_X:
		return XMVectorGetX(m_vInfomation[INFO_ANGLE]);
	case ANGLE_Y:
		return XMVectorGetY(m_vInfomation[INFO_ANGLE]);
	case ANGLE_Z:
		return XMVectorGetZ(m_vInfomation[INFO_ANGLE]);
	}
	return 0;
}

DirectX::XMVECTOR CTransform::GetDirection()
{
	return m_MatWorld.r[2];
}

void CTransform::SetPosition(XMVECTOR vPos)
{
	m_vInfomation[INFO_POSITION] = vPos;
}

void CTransform::SetPosition(float PosX, float PosY, float PosZ)
{
	m_vInfomation[INFO_POSITION] = XMVectorSet(PosX, PosY, PosZ, 1);
}

void CTransform::SetScale(XMVECTOR vScale)
{
	m_vInfomation[INFO_SCALE] = vScale;
}

void CTransform::SetScale(float scaleXYZ)
{
	m_vInfomation[INFO_SCALE] = XMVectorSet(scaleXYZ, scaleXYZ, scaleXYZ,0);
}

void CTransform::SetScale(float scaleX, float scaleY, float scaleZ)
{
	m_vInfomation[INFO_SCALE] = XMVectorSet(scaleX, scaleY, scaleZ, 0);
}

DirectX::XMVECTOR CTransform::GetPosition()
{
	return m_vInfomation[INFO_POSITION];
}

