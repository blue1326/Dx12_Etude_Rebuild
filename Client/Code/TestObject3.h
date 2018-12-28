#ifndef TestObject3_h__
#define TestObject3_h__
#include "stdHeaders.h"
#include "GameObject.h"
#include "Component.h"
#include "DxDevice.h"

class CTestObject3 : public CGameObject
{
public:
	explicit CTestObject3(shared_ptr<DxDevice> _device);
	virtual ~CTestObject3();
public:
	virtual HRESULT Init_GameObject(void)override;
	virtual int Update_GameObject(const std::shared_ptr<CTimer> t)override;
	virtual int LastUpdate_GameObject(const std::shared_ptr<CTimer> t)override;
	virtual void Render_GameObject()override;

	virtual void OnResize()override;
protected:
	
private:
	
	shared_ptr<CComponent> pBox;
	shared_ptr<CComponent> pTransform;
	shared_ptr<CComponent> pRenderer;
	shared_ptr<CComponent> pConstant;
private:
	CTestObject3();
};
 

#endif // TestObject3_h__
