#include "Material.h"
#include "dxException.h"
#include "DDSTextureLoader.h"
CMaterial::CMaterial(const shared_ptr<DxDevice> _device)
	:m_DxDevice(_device)
	,m_Material(nullptr)
	,m_Texture(nullptr)
{

}

CMaterial::CMaterial(const CMaterial& rhs)
	:m_DxDevice(rhs.m_DxDevice)
	,m_Material(rhs.m_Material)
	,m_Texture(rhs.m_Texture)
{

}

CMaterial::~CMaterial()
{

}

HRESULT CMaterial::Init_Component()
{
	if (m_Material != nullptr && m_Texture != nullptr)
	{
		Init_Start();


		LoadTexture();




		Init_End();
	}
	else
		return E_FAIL;
	return S_OK;
}

void CMaterial::Update_Component(const std::shared_ptr<CTimer> t)
{

}

std::shared_ptr<CComponent> CMaterial::Clone()
{
	return shared_ptr<CComponent>(new CMaterial(*this));
}

void CMaterial::SetUpMaterialState(const shared_ptr<Material> _Material)
{
	m_Material = _Material;
}

void CMaterial::SetUpTextureLocation(const wstring& _str)
{
	auto tex = make_shared<Texture>();
	tex->Name = "";
	tex->Filename = _str;
	m_Texture = tex;
}

void CMaterial::Init_Start()
{
	ThrowIfFailed(m_DxDevice->GetCommandList()->Reset(m_DxDevice->GetCommandAllocator().Get(), nullptr));
}

void CMaterial::Init_End()
{
	// Execute the initialization commands.
	ThrowIfFailed(m_DxDevice->GetCommandList()->Close());
	ID3D12CommandList* cmdsLists[] = { m_DxDevice->GetCommandList().Get() };
	m_DxDevice->GetCommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	m_DxDevice->FlushCommandQueue();
}

void CMaterial::LoadTexture()
{
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(m_DxDevice->GetDevice().Get(),
		m_DxDevice->GetCommandList().Get(), m_Texture->Filename.c_str(),
		m_Texture->Resource, m_Texture->UploadHeap));
}

void CMaterial::SetUpDescripterHeap(ComPtr<ID3D12DescriptorHeap> _heap)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(_heap->GetCPUDescriptorHandleForHeapStart());

	auto tex = m_Texture->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = tex->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = tex->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	m_DxDevice->GetDevice()->CreateShaderResourceView(tex.Get(), &srvDesc, hDescriptor);
}


