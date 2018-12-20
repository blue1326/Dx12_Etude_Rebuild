#include "Camera.h"
#include "ComponentHolder.h"
#include "Renderer.h"
#include "MathHelper.h"
#include "Structs.h"
#include "Timer.h"
CCamera::CCamera(const shared_ptr<DxDevice> _device)
	:m_DxDevice(_device)
	, m_fFovy(0.f)
	, m_fAspect(0.f)
	, m_fNear(0.f)
	, m_fFar(0.f)
{
	
}

CCamera::CCamera(const CCamera& rhs)
	:m_DxDevice(rhs.m_DxDevice)
	,m_vEye(rhs.m_vEye)
	,m_vAt(rhs.m_vAt)
	,m_vUp(rhs.m_vUp)
	,m_fFovy(rhs.m_fFovy)
	,m_fAspect(rhs.m_fAspect)
	,m_fNear(rhs.m_fNear)
	,m_fFar(rhs.m_fFar)
	,m_World(rhs.m_World)
	,m_View(rhs.m_View)
	,m_Proj(rhs.m_Proj)
{
	
}

CCamera::~CCamera()
{
	//m_d3dDevice = nullptr;
}

HRESULT CCamera::Init_Component()
{
	
	
	m_vEye = XMVectorSet(0, 0, 0, 0);
		//XMLoadFloat4(&initValue);
	m_vAt = XMVectorZero();
	m_vUp = XMVectorSet(0, 1, 0, 0);
	m_World = XMMatrixIdentity();
	m_View = XMMatrixIdentity();
	m_Proj = XMMatrixIdentity();
		
	
	return S_OK;
}

void CCamera::Update_Component(const std::shared_ptr<CTimer> t)
{
	                                   //pos   target  up
	XMMATRIX m_View = XMMatrixLookAtLH(m_vEye, m_vAt, m_vUp);
	//g_matView = m_View;
	
	m_DxDevice->SetViewMatrix(m_View);

	SetPassConstant(t);

}

std::shared_ptr<CComponent> CCamera::Clone()
{
	std::shared_ptr<CComponent> instance = std::shared_ptr<CComponent>(new CCamera(*this));
	//CopyComponentSettings(instance);
	return instance;
}


void CCamera::OnResize()
{
	m_Proj =  XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	//g_matProj = m_Proj;
	m_DxDevice->SetProjMatrix(m_Proj);
	/*XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_Proj, P);*/
}

void CCamera::SetEye(XMVECTOR vEye)
{
	m_vEye = vEye;
}

void CCamera::SetAt(XMVECTOR vAt)
{
	m_vAt = vAt;
}

void CCamera::SetUp(XMVECTOR vUp)
{
	m_vUp = vUp;
}

void CCamera::SetEyeAtUp(XMVECTOR vEye, XMVECTOR vAt, XMVECTOR vUp)
{
	m_vEye = vEye;
	m_vAt = vAt;
	m_vUp = vUp;
}


void CCamera::SetEyeAtUp(const XMVECTOR* vEye /*= nullptr*/, const XMVECTOR* vAt /*= nullptr*/, const XMVECTOR* vUp /*= nullptr*/)
{
	if (nullptr != vEye)
		m_vEye = *vEye;
	if (nullptr != vAt)
		m_vAt = *vAt;
	if (nullptr != vUp)
		m_vUp = *vUp;
}

void CCamera::SetPassConstant(const std::shared_ptr<CTimer> t)
{
	PassConstants _constant;
	XMMATRIX view = m_View;
	XMMATRIX proj = m_Proj;

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	XMStoreFloat4x4(&_constant.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&_constant.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&_constant.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&_constant.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&_constant.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&_constant.InvViewProj, XMMatrixTranspose(invViewProj));
	XMStoreFloat3(&_constant.EyePosW, m_vEye);//위험군
	//_constant.EyePosW = XMStoreFloat3(m_vEye); //위험군
	_constant.RenderTargetSize = XMFLOAT2((float)g_ClientWidth, (float)g_ClientHeight);
	_constant.InvRenderTargetSize = XMFLOAT2(1.0f / g_ClientWidth, 1.0f / g_ClientHeight);
	_constant.NearZ = 1.0f;
	_constant.FarZ = 1000.0f;
	_constant.TotalTime = t->TotalTime();
	_constant.DeltaTime = t->DeltaTime();
	_constant.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	_constant.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	_constant.Lights[0].Strength = { 0.6f, 0.6f, 0.6f };
	_constant.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	_constant.Lights[1].Strength = { 0.3f, 0.3f, 0.3f };
	_constant.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	_constant.Lights[2].Strength = { 0.15f, 0.15f, 0.15f };

	m_DxDevice->SetPassConstant(_constant);
}

void CCamera::SetWorld(XMMATRIX matWorld)
{
	//m_World = matWorld;
}

DirectX::XMVECTOR CCamera::GetEye()
{
	return m_vEye;
}

DirectX::XMVECTOR CCamera::GetAt()
{
	return m_vAt;
}

DirectX::XMVECTOR CCamera::GetUp()
{
	return m_vUp;
}
//
//void CCamera::GetEyeAtUp(XMVECTOR& vEye, XMVECTOR& vAt, XMVECTOR& vUp)
//{
//	vEye = m_vEye;
//	vAt = m_vAt;
//	vUp = m_vUp;
//}

void CCamera::GetEyeAtUp( XMVECTOR* vEye /*=nullptr*/,  XMVECTOR* vAt /*= nullptr*/,  XMVECTOR* vUp /*= nullptr*/)
{
	if(nullptr != vEye)
		*vEye = m_vEye;
	if(nullptr != vAt)
		*vAt = m_vAt;
	if (nullptr != vUp)
		*vUp = m_vUp;
}

