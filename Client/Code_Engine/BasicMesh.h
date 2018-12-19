#ifndef BasicMesh_h__
#define BasicMesh_h__

///it is not using yet
///maybe i will using inherit under class
#include "Component.h"
#include "DxDevice.h"
#include "Structs.h"
#include "MeshGeometry.h"
class CBasicMesh : public CComponent
{
protected:
	explicit CBasicMesh(const shared_ptr<DxDevice> _device);
	//explicit CBasicMesh(const CBasicMesh& rhs);
	virtual ~CBasicMesh();


public:
	virtual HRESULT Init_Component()override;
	virtual void Update_Component(const std::shared_ptr<CTimer> t)override;

private:
	virtual void BuildGeometry();
protected:
	const shared_ptr<DxDevice> m_DxDevice;

protected:
	void Init_Start();
	void Init_End();
public:
	virtual std::shared_ptr<CComponent> Clone()override;
protected:
	shared_ptr<MeshGeometry> m_Geometry;

public:
	shared_ptr<MeshGeometry> GetGeometry()
	{
		return m_Geometry;
	}




private:
	void Subdivide(MeshData& meshData);
	VTX MidPoint(const VTX& v0, const VTX& v1);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);

protected:///generate basic mesh geometry 
	///<summary>
	/// Creates a box centered at the origin with the given dimensions, where each
	/// face has m rows and n columns of vertices.
	///</summary>
	MeshData CreateBox(float width, float height, float depth, uint32_t numSubdivisions);

	///<summary>
	/// Creates a sphere centered at the origin with the given radius.  The
	/// slices and stacks parameters control the degree of tessellation.
	///</summary>
	MeshData CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount);

	///<summary>
	/// Creates a geosphere centered at the origin with the given radius.  The
	/// depth controls the level of tessellation.
	///</summary>
	MeshData CreateGeosphere(float radius, uint32_t numSubdivisions);

	///<summary>
	/// Creates a cylinder parallel to the y-axis, and centered about the origin.  
	/// The bottom and top radius can vary to form various cone shapes rather than true
	// cylinders.  The slices and stacks parameters control the degree of tessellation.
	///</summary>
	MeshData CreateCylinder(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount);

	///<summary>
	/// Creates an mxn grid in the xz-plane with m rows and n columns, centered
	/// at the origin with the specified width and depth.
	///</summary>
	MeshData CreateGrid(float width, float depth, uint32_t m, uint32_t n);

	///<summary>
	/// Creates a quad aligned with the screen.  This is useful for postprocessing and screen effects.
	///</summary>
	MeshData CreateQuad(float x, float y, float w, float h, float depth);
};



#endif // BasicMesh_h__
