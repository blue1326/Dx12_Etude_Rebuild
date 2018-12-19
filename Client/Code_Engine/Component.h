#ifndef Component_h__
#define Component_h__
#include "stdHeaders.h"
class DxDevice;
class CTimer;
class CComponent : public std::enable_shared_from_this<CComponent>
{
protected:
	explicit CComponent();
	explicit CComponent(const CComponent& rhs) = delete;
	virtual ~CComponent();
	
public:
	virtual HRESULT Init_Component();
	virtual void Update_Component(const std::shared_ptr<CTimer> t);
	virtual std::shared_ptr<CComponent> Clone()=0;
	virtual void OnResize();

	

	
		
};

#endif // Component_h__
