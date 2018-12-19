#ifndef Camera_h__
#define Camera_h__

#include "stdHeaders.h"
#include "DxDevice.h"
#include "Component.h"

class CCamera :public CComponent
{
public:
	explicit CCamera(const shared_ptr<DxDevice> _device);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();
public:
	virtual HRESULT Init_Component()override;
	virtual void Update_Component(const std::shared_ptr<CTimer> t)override;
	virtual std::shared_ptr<CComponent> Clone()override;
public:
	virtual void OnResize()override;
protected:
	
private:
	XMVECTOR m_vEye, m_vAt, m_vUp;
	float	m_fFovy, m_fAspect, m_fNear, m_fFar;

	/*XMFLOAT4X4 m_World = MathHelper::Identity4x4();
	XMFLOAT4X4 m_View = MathHelper::Identity4x4();
	XMFLOAT4X4 m_Proj = MathHelper::Identity4x4();*/

	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Proj;
	/*float mTheta = 1.5f*XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;*/
public:
	void SetEye(XMVECTOR vEye);//position
	void SetAt(XMVECTOR vAt);//target
	void SetUp(XMVECTOR vUp);
	void SetEyeAtUp(XMVECTOR vEye, XMVECTOR vAt, XMVECTOR vUp);
	void SetWorld(XMMATRIX matWorld);

private:
	const shared_ptr<DxDevice> m_DxDevice;
};


#endif // Camera_h__
