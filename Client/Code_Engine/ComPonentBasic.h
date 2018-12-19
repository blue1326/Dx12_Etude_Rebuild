#ifndef Box_h__
#define Box_h__

#include "stdHeaders.h"
#include "Component.h"

class CBox :public CComponent
{
public:
	explicit CBox();
	virtual ~CBox();
public:
	virtual bool Init_Component()override;
	virtual void Update_Component()override;
	virtual std::shared_ptr<CComponent> Clone()override;
private:
	virtual void CopyComponentSettings(std::shared_ptr<CComponent> CopyInst)override;
	virtual void OnResize()override;
protected:

private:
	
};


#endif // Box_h__
