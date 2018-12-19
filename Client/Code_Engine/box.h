#ifndef box_h__
#define box_h__

#include "Component.h"
#include "DxDevice.h"
#include "MeshGeometry.h"
#include "Structs.h"
#include "MathHelper.h"

//struct Vertex
//{
//	XMFLOAT3 Pos;
//	XMFLOAT4 Color;
//};
//
//struct ObjectConstants
//{
//	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
//};

class CBox : public CComponent
{
public:
	explicit CBox(const shared_ptr<DxDevice> _device);
	explicit CBox(const CBox& rhs);
	virtual ~CBox();

public:
	virtual HRESULT Init_Component()override;
	virtual void Update_Component(const std::shared_ptr<CTimer> t)override;

private:
	void BuildBoxGeometry();

private:
	const shared_ptr<DxDevice> m_DxDevice;

	std::shared_ptr<MeshGeometry> m_BoxGeo;

public:
	shared_ptr<MeshGeometry> GetGeometry()
	{
		return m_BoxGeo;
	}
public:
	virtual std::shared_ptr<CComponent> Clone()override;
private:

};



#endif // box_h__
