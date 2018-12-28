#ifndef TestObject_h__
#define TestObject_h__
#include "stdHeaders.h"
#include "GameObject.h"
#include "Component.h"
#include "DxDevice.h"

class CTestObject : public CGameObject
{
public:
	explicit CTestObject(shared_ptr<DxDevice> _device);
	virtual ~CTestObject();
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
	shared_ptr<CComponent> pDescriptorHeap;
	shared_ptr<CComponent> pConstantBuffer;
private:
	CTestObject();
};
 

#endif // TestObject_h__
