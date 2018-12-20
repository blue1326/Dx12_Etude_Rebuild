#ifndef TestObject_Camera_h__
#define TestObject_Camera_h__
#include "stdHeaders.h"
#include "GameObject.h"
#include "Component.h"
#include "DxDevice.h"


class CTestObject_Camera : public CGameObject
{
public:
	explicit	CTestObject_Camera(shared_ptr<DxDevice> _device);
	~CTestObject_Camera();
public:
	virtual HRESULT Init_GameObject(void)override;
	virtual int Update_GameObject(const std::shared_ptr<CTimer> t)override;
	virtual int LastUpdate_GameObject(const std::shared_ptr<CTimer> t)override;
	virtual void Render_GameObject()override;
	virtual void OnResize()override;


private:///is testcode soon move and remake
	void Check_input_Keyboard(const std::shared_ptr<CTimer> t);
	void Check_input_Mouse(const std::shared_ptr<CTimer> t);


protected:
	
private:
	shared_ptr<CComponent> pTransform;
	shared_ptr<CComponent> pCamera;
	shared_ptr<CComponent> pController;
};
 

#endif // TestObject_Camera_h__
