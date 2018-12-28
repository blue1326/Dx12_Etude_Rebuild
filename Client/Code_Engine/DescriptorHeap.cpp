#include "DescriptorHeap.h"
#include "dxException.h"
CDescriptorHeap::CDescriptorHeap(const shared_ptr<DxDevice> _device)
	: m_DxDevice(_device)
	,m_DescriptorSize(0)
	, m_HeapDesc({})
	,isDescSetup(false)
{

}

CDescriptorHeap::~CDescriptorHeap()
{

}

HRESULT CDescriptorHeap::Init_Component()
{
	m_DescriptorSize = m_DxDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	BuildDescriptorHeap();

	return S_OK;
}

void CDescriptorHeap::Update_Component()
{

}

std::shared_ptr<CComponent> CDescriptorHeap::Clone()
{
	return shared_ptr<CComponent>(new CDescriptorHeap(m_DxDevice));
}

void CDescriptorHeap::OnResize()
{

}

void CDescriptorHeap::BuildDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = {};
	if (!isDescSetup)
	{
		HeapDesc.NumDescriptors = 1;
		HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		HeapDesc.NodeMask = 0;
	}
	else
	{
		HeapDesc = m_HeapDesc;
	}
	ThrowIfFailed(m_DxDevice->GetDevice()->CreateDescriptorHeap(&HeapDesc,
		IID_PPV_ARGS(&m_Heap)));
}

void CDescriptorHeap::SetUpHeapDesc(D3D12_DESCRIPTOR_HEAP_DESC& _desc)
{
	m_HeapDesc = _desc;
	isDescSetup = true;
}

