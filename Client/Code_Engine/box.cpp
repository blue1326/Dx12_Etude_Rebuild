#include "box.h"
#include "dxException.h"
//#include "BufferCreater.h"
#include "ShaderCompiler.h"
#include "BufferUtility.h"
//#include "dxException.h"
CBox::CBox(const shared_ptr<DxDevice> _device)
	:m_DxDevice(_device)
	,m_BoxGeo(nullptr)
{

}

CBox::CBox(const CBox& rhs)
	:m_DxDevice(rhs.m_DxDevice)
	,m_BoxGeo(rhs.m_BoxGeo)
{
	
}

CBox::~CBox()
{

}

HRESULT CBox::Init_Component()
{
	ThrowIfFailed(m_DxDevice->GetCommandList()->Reset(m_DxDevice->GetCommandAllocator().Get(), nullptr));

	BuildBoxGeometry();

	// Execute the initialization commands.
	ThrowIfFailed(m_DxDevice->GetCommandList()->Close());
	ID3D12CommandList* cmdsLists[] = { m_DxDevice->GetCommandList().Get() };
	m_DxDevice->GetCommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	m_DxDevice->FlushCommandQueue();
	return S_OK;
}

void CBox::Update_Component(const std::shared_ptr<CTimer> t)
{
}


void CBox::BuildBoxGeometry()
{
	std::array<Vertex, 8> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	m_BoxGeo = std::make_shared<MeshGeometry>();
	m_BoxGeo->Name = "boxGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &m_BoxGeo->VertexBufferCPU));
	CopyMemory(m_BoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &m_BoxGeo->IndexBufferCPU));
	CopyMemory(m_BoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	m_BoxGeo->VertexBufferGPU = BufferUtility::CreateDefaultBuffer(m_DxDevice->GetDevice().Get(),
		m_DxDevice->GetCommandList().Get(), vertices.data(), vbByteSize, m_BoxGeo->VertexBufferUploader);

	m_BoxGeo->IndexBufferGPU = BufferUtility::CreateDefaultBuffer(m_DxDevice->GetDevice().Get(),
		m_DxDevice->GetCommandList().Get(), indices.data(), ibByteSize, m_BoxGeo->IndexBufferUploader);

	m_BoxGeo->VertexByteStride = sizeof(Vertex);
	m_BoxGeo->VertexBufferByteSize = vbByteSize;
	m_BoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	m_BoxGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	m_BoxGeo->DrawArgs["box"] = submesh;
}


std::shared_ptr<CComponent> CBox::Clone()
{
	return this->shared_from_this();
}

// it is moved to gameobject
//void CBox::CreateUplaodBuffer()
//{
//	int elementCount = 1;
//	// Constant buffer elements need to be multiples of 256 bytes.
//	// This is because the hardware can only view constant data 
//	// at m*256 byte offsets and of n*256 byte lengths. 
//	// typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
//	// UINT64 OffsetInBytes; // multiple of 256
//	// UINT   SizeInBytes;   // multiple of 256
//	// } D3D12_CONSTANT_BUFFER_VIEW_DESC;
//	//it is now using just constant buffer
//	m_ElementByteSize = BufferUtility::CalcConstantBufferByteSize(sizeof(ObjectConstants));
//
//	ThrowIfFailed(m_DxDevice->GetDevice().Get()->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize*elementCount),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&m_UploadBuffer)));
//
//	ThrowIfFailed(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
//
//	// We do not need to unmap until we are done with the resource.  However, we must not write to
//	// the resource while it is in use by the GPU (so we must use synchronization techniques).
//
//
//
//}
//
//void CBox::BuildDescriptorHeaps()
//{
//	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
//	cbvHeapDesc.NumDescriptors = 1;
//	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//	cbvHeapDesc.NodeMask = 0;
//	ThrowIfFailed(m_DxDevice->GetDevice()->CreateDescriptorHeap(&cbvHeapDesc,
//		IID_PPV_ARGS(&m_CbvHeap)));
//}

//void CBox::BuildConstantBuffers()
//{
//	CreateUplaodBuffer();
//
//	//UINT objCBByteSize = BufferUtility::CalcConstantBufferByteSize(sizeof(ObjectConstants));
//	//m_emementbytesize
//	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = m_UploadBuffer.Get()->GetGPUVirtualAddress();
//	//D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress(); dxcookbook Original
//	int boxCBufIndex = 0;
//	cbAddress += boxCBufIndex * m_ElementByteSize; //extend size
//
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
//	cbvDesc.BufferLocation = cbAddress;
//	cbvDesc.SizeInBytes = m_ElementByteSize;
//
//	m_DxDevice->GetDevice().Get()->CreateConstantBufferView(&cbvDesc, m_CbvHeap->GetCPUDescriptorHandleForHeapStart());
//}


//move to renderer

//
//void CBox::BuildRootSignature()
//{
//	// Shader programs typically require resources as input (constant buffers,
//	// textures, samplers).  The root signature defines the resources the shader
//	// programs expect.  If we think of the shader programs as a function, and
//	// the input resources as function parameters, then the root signature can be
//	// thought of as defining the function signature.  
//
//	// Root parameter can be a table, root descriptor or root constants.
//	CD3DX12_ROOT_PARAMETER slotRootParameter[1];
//
//	// Create a single descriptor table of CBVs.
//	CD3DX12_DESCRIPTOR_RANGE cbvTable;
//	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
//	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);
//
//	// A root signature is an array of root parameters.
//	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
//		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
//
//	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
//	ComPtr<ID3DBlob> serializedRootSig = nullptr;
//	ComPtr<ID3DBlob> errorBlob = nullptr;
//	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
//		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
//
//	if (errorBlob != nullptr)
//	{
//		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
//	}
//	ThrowIfFailed(hr);
//
//	ThrowIfFailed(m_DxDevice->GetDevice()->CreateRootSignature(
//		0,
//		serializedRootSig->GetBufferPointer(),
//		serializedRootSig->GetBufferSize(),
//		IID_PPV_ARGS(&m_RootSignature)));
//}
//
//void CBox::BuildShadersAndInputLayout()
//{
//	m_vsByteCode = CShaderCompiler::CompileShader(L"../Shaders/Box_color.hlsl", nullptr, "VS", "vs_5_0");
//	m_psByteCode = CShaderCompiler::CompileShader(L"../Shaders/Box_color.hlsl", nullptr, "PS", "ps_5_0");
//
//	m_InputLayout =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
//	};
//}

//
//
//void CBox::BuildPSO()
//{
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
//	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
//	psoDesc.InputLayout = { m_InputLayout.data(), (UINT)m_InputLayout.size() };
//	psoDesc.pRootSignature = m_RootSignature.Get();
//	psoDesc.VS =
//	{
//		reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()),
//		m_vsByteCode->GetBufferSize()
//	};
//	psoDesc.PS =
//	{
//		reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()),
//		m_psByteCode->GetBufferSize()
//	};
//	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
//	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
//	psoDesc.SampleMask = UINT_MAX;
//	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//	psoDesc.NumRenderTargets = 1;
//	psoDesc.RTVFormats[0] = m_DxDevice->GetBackBufferFormat();
//	psoDesc.SampleDesc.Count = m_DxDevice->Get4xMsaaState() ? 4 : 1;
//	psoDesc.SampleDesc.Quality = m_DxDevice->Get4xMsaaState() ? (m_DxDevice->Get4xMsaaQuality() - 1) : 0;
//	psoDesc.DSVFormat = m_DxDevice->GetDepthStencilBufferFormat();
//	ThrowIfFailed(m_DxDevice->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO)));
//}