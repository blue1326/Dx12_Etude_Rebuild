#include "BasicMesh_Crate.h"
//#include "Structs.h"
#include "MeshGeometry.h"
#include "dxException.h"
#include "BufferUtility.h"
CBasicMesh_Crate::CBasicMesh_Crate(const shared_ptr<DxDevice> _device)
	:CBasicMesh::CBasicMesh(_device)
{

}
//
//CBasicMesh_Crate::CBasicMesh_Crate(const CBasicMesh_Crate& rhs)
//	:CBasicMesh::CBasicMesh(rhs.m_DxDevice)
//{
//
//}

CBasicMesh_Crate::~CBasicMesh_Crate()
{

}

HRESULT CBasicMesh_Crate::Init_Component()
{
	Init_Start();

	BuildGeometry();

	Init_End();
	return S_OK;
}

void CBasicMesh_Crate::Update_Component(const std::shared_ptr<CTimer> t)
{

}

void CBasicMesh_Crate::BuildGeometry()
{
	MeshData box = CreateBox(1, 1, 1, 3);

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = 0;
	boxSubmesh.BaseVertexLocation = 0;


	std::vector<Vertex> vertices(box.Vertices.size());

	for (size_t i = 0; i < box.Vertices.size(); ++i)
	{
		vertices[i].Pos = box.Vertices[i].Position;
		vertices[i].Normal = box.Vertices[i].Normal;
		vertices[i].TexC = box.Vertices[i].TexC;
	}

	std::vector<std::uint16_t> indices = box.GetIndices16();

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_shared<MeshGeometry>();
	geo->Name = "boxGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = BufferUtility::CreateDefaultBuffer(m_DxDevice->GetDevice().Get(),
		m_DxDevice->GetCommandList().Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = BufferUtility::CreateDefaultBuffer(m_DxDevice->GetDevice().Get(),
		m_DxDevice->GetCommandList().Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	//geo->DrawArgs["box"] = boxSubmesh;
	geo->DrawArg = boxSubmesh;

	m_Geometry = geo;
}

std::shared_ptr<CComponent> CBasicMesh_Crate::Clone()
{
	return this->shared_from_this();
}