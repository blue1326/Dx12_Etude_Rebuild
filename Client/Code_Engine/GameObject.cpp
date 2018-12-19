#include "GameObject.h"
#include "Structs.h"
#include "dxException.h"
#include "BufferUtility.h"
CGameObject::CGameObject(shared_ptr<DxDevice> _device)
	:m_DxDevice(_device)
	,m_ElementByteSize(0)
	,m_MappedData(nullptr)
	,m_CbvHeap(nullptr)
{

}

CGameObject::~CGameObject()
{

}

void CGameObject::Free(void)
{

}

void CGameObject::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(m_DxDevice->GetDevice()->CreateDescriptorHeap(&cbvHeapDesc,
		IID_PPV_ARGS(&m_CbvHeap)));
}

void CGameObject::BuildConstantBuffers()
{
	CreateUplaodBuffer();
	//UINT objCBByteSize = BufferUtility::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	//m_emementbytesize
	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = m_UploadBuffer.Get()->GetGPUVirtualAddress();
	//D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress(); dxcookbook Original
	int boxCBufIndex = 0;
	cbAddress += boxCBufIndex * m_ElementByteSize; //extend size

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = m_ElementByteSize;

	m_DxDevice->GetDevice().Get()->CreateConstantBufferView(&cbvDesc, m_CbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void CGameObject::CreateUplaodBuffer()
{
	//int elementCount = 1;
	// Constant buffer elements need to be multiples of 256 bytes.
	// This is because the hardware can only view constant data 
	// at m*256 byte offsets and of n*256 byte lengths. 
	// typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
	// UINT64 OffsetInBytes; // multiple of 256
	// UINT   SizeInBytes;   // multiple of 256
	// } D3D12_CONSTANT_BUFFER_VIEW_DESC;
	//it is now using just constant buffer
	m_ElementByteSize = BufferUtility::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	ThrowIfFailed(m_DxDevice->GetDevice().Get()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_UploadBuffer)));

	ThrowIfFailed(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));

	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

