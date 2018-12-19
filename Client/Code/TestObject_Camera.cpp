#include "TestObject_Camera.h"
#include "ComponentHolder.h"

#include "Transform.h"
#include "Camera.h"



CTestObject_Camera::CTestObject_Camera(shared_ptr<DxDevice> _device)
	:CGameObject::CGameObject(_device)
{

}

CTestObject_Camera::~CTestObject_Camera()
{

}

HRESULT CTestObject_Camera::Init_GameObject(void)
{
	pTransform = CComponentHolder::GetInstance()->Clone_Component("Transform");
	((CTransform*)pTransform.get())->Init_Component();
	((CTransform*)pTransform.get())->SetPosition(4, -3, 3);
	pCamera = CComponentHolder::GetInstance()->Clone_Component("Camera");
	((CCamera*)pCamera.get())->Init_Component();
	return S_OK;
}

int CTestObject_Camera::Update_GameObject(const std::shared_ptr<CTimer> t)
{
	dynamic_cast<CCamera*>(pCamera.get())->SetEye(((CTransform*)pTransform.get())->GetPosition());
	dynamic_cast<CCamera*>(pCamera.get())->Update_Component(t);
	
	return 0;
}

int CTestObject_Camera::LastUpdate_GameObject(const std::shared_ptr<CTimer> t)
{
	
	return 0;
}

void CTestObject_Camera::Render_GameObject()
{
	
	
}

void CTestObject_Camera::OnResize()
{
	dynamic_cast<CCamera*>(pCamera.get())->OnResize();
}

