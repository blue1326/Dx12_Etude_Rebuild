#ifndef BasicMesh_Crate_h__
#define BasicMesh_Crate_h__

///it is not using yet
///maybe i will using inherit under class
//#include "Component.h"
#include "BasicMesh.h"
#include "DxDevice.h"
//#include "Structs.h"
class CBasicMesh_Crate : public CBasicMesh
{
public:
	explicit CBasicMesh_Crate(const shared_ptr<DxDevice> _device);
	//explicit CBasicMesh_Crate(const CBasicMesh_Crate& rhs);
	virtual ~CBasicMesh_Crate();


public:
	virtual HRESULT Init_Component()override;
	virtual void Update_Component(const std::shared_ptr<CTimer> t)override;

private:
	virtual void BuildGeometry();



public:
	virtual std::shared_ptr<CComponent> Clone()override;


};



#endif // BasicMesh_Crate_h__
