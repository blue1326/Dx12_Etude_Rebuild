#include "Renderer.h"
#include "dxException.h"
#include "ShaderCompiler.h"
CRenderer::CRenderer(const std::shared_ptr<DxDevice> _device)
	:m_DxDevice(_device)
	,m_RootSignature(nullptr)
{
	
}

CRenderer::~CRenderer()
{

}

HRESULT CRenderer::Init_Component()
{
	ThrowIfFailed(m_DxDevice->GetCommandList()->Reset(m_DxDevice->GetCommandAllocator().Get(), nullptr));

	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildPSOs();

	// Execute the initialization commands.
	ThrowIfFailed(m_DxDevice->GetCommandList()->Close());
	ID3D12CommandList* cmdsLists[] = { m_DxDevice->GetCommandList().Get() };
	m_DxDevice->GetCommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	m_DxDevice->FlushCommandQueue();
	return S_OK;
}

void CRenderer::Update_Component(const std::shared_ptr<CTimer> t)
{

}

void CRenderer::Render_GameObject(void)
{
	RenderNoneAlpha();
	Clear_RenderList();
}

void CRenderer::RenderNoneAlpha(void)
{
	//m_DxDevice->GetCommandList().Get()->SetPipelineState(GetPSO("opaquePSO").Get());
	m_DxDevice->GetCommandList().Get()->SetPipelineState(GetPSO("opaque_wf").Get());
	for (const auto &j : m_RenderList[RENDER_NONEALPHA])
	{
		j->Render_GameObject();
	}
}
std::shared_ptr<CComponent> CRenderer::Clone()
{
	return this->shared_from_this();
}

HRESULT CRenderer::Add_RenderList(RENDER eType, shared_ptr<CGameObject> object)
{
	if (nullptr == object)
		return E_FAIL;
	m_RenderList[eType].push_back(object);
	return S_OK;
}

void CRenderer::Clear_RenderList()
{
	for (unsigned long i = 0; i < RENDER_END; ++i)
	{
		RENDERLIST::iterator erBegin = m_RenderList[i].begin();
		RENDERLIST::iterator erEnd = m_RenderList[i].end();
		m_RenderList[i].erase(erBegin, erEnd);
		m_RenderList[i].clear();
	}
}

void CRenderer::BuildRootSignature()
{
	// Shader programs typically require resources as input (constant buffers,
	// textures, samplers).  The root signature defines the resources the shader
	// programs expect.  If we think of the shader programs as a function, and
	// the input resources as function parameters, then the root signature can be
	// thought of as defining the function signature.  

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	// Create a single descriptor table of CBVs.
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(m_DxDevice->GetDevice()->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&m_RootSignature)));
}

void CRenderer::BuildShadersAndInputLayout()
{
	m_Shaders["stdVS"] = CShaderCompiler::CompileShader(L"../Shaders/Box_color.hlsl", nullptr, "VS", "vs_5_0");
	//m_vsByteCode = CShaderCompiler::CompileShader(L"../Shaders/Box_color.hlsl", nullptr, "VS", "vs_5_0");
	m_Shaders["opaquePS"] = CShaderCompiler::CompileShader(L"../Shaders/Box_color.hlsl", nullptr, "PS", "ps_5_0");
	//m_psByteCode = CShaderCompiler::CompileShader(L"../Shaders/Box_color.hlsl", nullptr, "PS", "ps_5_0");

	m_InputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

void CRenderer::BuildPSOs()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_InputLayout.data(), (UINT)m_InputLayout.size() };
	psoDesc.pRootSignature = m_RootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["stdVS"]->GetBufferPointer()),
		m_Shaders["stdVS"]->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["opaquePS"]->GetBufferPointer()),
		m_Shaders["opaquePS"]->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = m_DxDevice->GetBackBufferFormat();
	psoDesc.SampleDesc.Count = m_DxDevice->Get4xMsaaState() ? 4 : 1;
	psoDesc.SampleDesc.Quality = m_DxDevice->Get4xMsaaState() ? (m_DxDevice->Get4xMsaaQuality() - 1) : 0;
	psoDesc.DSVFormat = m_DxDevice->GetDepthStencilBufferFormat();
	ThrowIfFailed(m_DxDevice->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSOs["opaque"])));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaqueWireframePsoDesc = psoDesc;
	opaqueWireframePsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	ThrowIfFailed(m_DxDevice->GetDevice()->CreateGraphicsPipelineState(&opaqueWireframePsoDesc, IID_PPV_ARGS(&m_PSOs["opaque_wf"])));
}
