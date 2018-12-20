#include "TestObject_Camera.h"
#include "ComponentHolder.h"

#include "Transform.h"
#include "Camera.h"
#include "Timer.h"
#include "Controller.h"

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
	dynamic_cast<CCamera*>(pCamera.get())->SetEye(((CTransform*)pTransform.get())->GetPosition());


	pController = CComponentHolder::GetInstance()->Clone_Component("Controller");


	return S_OK;
}

int CTestObject_Camera::Update_GameObject(const std::shared_ptr<CTimer> t)
{
	Check_input_Keyboard(t);
	Check_input_Mouse(t);


	//dynamic_cast<CCamera*>(pCamera.get())->SetEye(((CTransform*)pTransform.get())->GetPosition());
	dynamic_cast<CCamera*>(pCamera.get())->Update_Component(t);
	dynamic_cast<CTransform*>(pTransform.get())->SetPosition(dynamic_cast<CCamera*>(pCamera.get())->GetEye());

		
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

void CTestObject_Camera::Check_input_Keyboard(const std::shared_ptr<CTimer> t)
{
	//it is testcode
	XMMATRIX matWorld;
	matWorld = XMMatrixInverse(nullptr, m_DxDevice->GetViewMatrix());
	XMFLOAT4X4 tmpWorld;
	XMStoreFloat4x4(&tmpWorld, matWorld);
	float fSpeed = 10.f;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		XMFLOAT3  vLook;
		memcpy(&vLook, &tmpWorld.m[2][0], sizeof(XMFLOAT3));

		//cout << vLook.x << " " << vLook.y << " " << vLook.z<<endl;

		XMVECTOR vEye, vAt;

		//XMFLOAT3  vAt, vEye;
		dynamic_cast<CCamera*>(pCamera.get())->GetEyeAtUp(&vEye, &vAt);
		XMVECTOR tmpLook = XMLoadFloat3(&vLook);
		
		XMVECTOR vLength = XMVector3Normalize(tmpLook) * fSpeed * t->DeltaTime();

		vEye += vLength;
		vAt += vLength;
		/*cout << vEye << endl;
		cout << vAt << endl;*/


		dynamic_cast<CCamera*>(pCamera.get())->SetEyeAtUp(&vEye, &vAt);
		
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		XMFLOAT3  vLook;
		memcpy(&vLook, &tmpWorld.m[2][0], sizeof(XMFLOAT3));

		//cout << vLook.x << " " << vLook.y << " " << vLook.z<<endl;

		XMVECTOR vEye, vAt;

		//XMFLOAT3  vAt, vEye;
		dynamic_cast<CCamera*>(pCamera.get())->GetEyeAtUp(&vEye, &vAt);
		XMVECTOR tmpLook = XMLoadFloat3(&vLook);

		XMVECTOR vLength = XMVector3Normalize(tmpLook) * fSpeed * t->DeltaTime();

		vEye -= vLength;
		vAt -= vLength;
		/*cout << vEye << endl;
		cout << vAt << endl;*/


		dynamic_cast<CCamera*>(pCamera.get())->SetEyeAtUp(&vEye, &vAt);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		XMFLOAT3  vLook;
		memcpy(&vLook, &tmpWorld.m[0][0], sizeof(XMFLOAT3));

		//cout << vLook.x << " " << vLook.y << " " << vLook.z<<endl;

		XMVECTOR vEye, vAt;

		//XMFLOAT3  vAt, vEye;
		dynamic_cast<CCamera*>(pCamera.get())->GetEyeAtUp(&vEye, &vAt);
		XMVECTOR tmpLook = XMLoadFloat3(&vLook);

		XMVECTOR vLength = XMVector3Normalize(tmpLook) * fSpeed * t->DeltaTime();

		vEye -= vLength;
		vAt -= vLength;
		/*cout << vEye << endl;
		cout << vAt << endl;*/


		dynamic_cast<CCamera*>(pCamera.get())->SetEyeAtUp(&vEye, &vAt);
	}

	
		
	if (GetAsyncKeyState('D') & 0x8000)
	{
		XMFLOAT3  vLook;
		memcpy(&vLook, &tmpWorld.m[0][0], sizeof(XMFLOAT3));

		//cout << vLook.x << " " << vLook.y << " " << vLook.z<<endl;

		XMVECTOR vEye, vAt;

		//XMFLOAT3  vAt, vEye;
		dynamic_cast<CCamera*>(pCamera.get())->GetEyeAtUp(&vEye, &vAt);
		XMVECTOR tmpLook = XMLoadFloat3(&vLook);

		XMVECTOR vLength = XMVector3Normalize(tmpLook) * fSpeed * t->DeltaTime();

		vEye += vLength;
		vAt += vLength;
		/*cout << vEye << endl;
		cout << vAt << endl;*/


		dynamic_cast<CCamera*>(pCamera.get())->SetEyeAtUp(&vEye, &vAt);
	}
	
}

void CTestObject_Camera::Check_input_Mouse(const std::shared_ptr<CTimer> t)
{

	///it is testcode
	CController* pCtrInst = dynamic_cast<CController*>(pController.get());
	CCamera* pCamInst = dynamic_cast<CCamera*>(pCamera.get());
	//signed long dwMouseMove = 0;
	XMMATRIX matWorld;
	matWorld = XMMatrixInverse(nullptr, m_DxDevice->GetViewMatrix());
	XMFLOAT4X4 tmpWorld;
	XMStoreFloat4x4(&tmpWorld, matWorld);
	float fSpeed = 100.f;
	//////
	XMVECTOR vAt, vEye;

	pCamInst->GetEyeAtUp(&vEye, &vAt);

	XMVECTOR vLook = vAt - vEye;
	XMMATRIX matRot;
	
	int moveX, moveY;
	pCtrInst->Get_MouseMoveXY(moveX, moveY);
	////axis X
	if (moveX != 0) {
		XMFLOAT3 tmpAxis = XMFLOAT3(0, 1, 0);
		XMVECTOR vAxis = XMLoadFloat3(&tmpAxis);
		matRot = XMMatrixRotationAxis(vAxis, XMConvertToRadians(moveX / 10.f));
		vLook = XMVector3TransformNormal(vLook, matRot);

		vAt = vEye + vLook;

		pCamInst->SetAt(vAt);
	}
	if (moveY != 0) {
		XMFLOAT3 tmpRight;
		memcpy(&tmpRight, &tmpWorld.m[0][0], sizeof(XMFLOAT3));
		XMVECTOR vRight = XMLoadFloat3(&tmpRight);
		

		matRot = XMMatrixRotationAxis(vRight, XMConvertToRadians(moveY / 10.f));
		vLook = XMVector3TransformNormal(vLook, matRot);

		vAt = vEye + vLook;
		pCamInst->SetAt(vAt);
	}

	

}

