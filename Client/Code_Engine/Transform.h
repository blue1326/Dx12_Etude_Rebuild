#ifndef Transform_h__
#define Transform_h__
#include "Component.h"
#include "DxDevice.h"
class CTransform : public CComponent
{
public:
	enum ANGLE { ANGLE_X, ANGLE_Y, ANGLE_Z };
	enum INFOMATION { INFO_SCALE, INFO_ANGLE, INFO_POSITION, INFO_END };
public:
	explicit CTransform(const shared_ptr<DxDevice> _device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();
public:
	virtual HRESULT Init_Component()override;
	virtual void Update_Component(const std::shared_ptr<CTimer> t)override;
	
public:
	virtual std::shared_ptr<CComponent> Clone()override;
	//virtual void OnResize()override;


private:
	XMVECTOR m_vInfomation[INFO_END];
	XMMATRIX m_MatWorld;
	XMMATRIX m_MatWorld_NonRot;
	XMMATRIX* m_pParentMatrix;
	

	///for cameraShake
	/*float m_OscillationScale;
	float m_OscillationValue;*/

public:
	XMMATRIX GetWorld();
	float GetAngle(ANGLE eType);
	XMVECTOR GetDirection();
	void SetPosition(XMVECTOR vPos);
	void SetPosition(float PosX, float PosY, float PosZ);
	void SetScale(XMVECTOR vScale);
	void SetScale(float scaleX, float scaleY, float scaleZ);
	void SetScale(float scaleXYZ);
	XMVECTOR GetPosition();


private:
	const shared_ptr<DxDevice> m_DxDevice;

};
#endif // Transform_h__
