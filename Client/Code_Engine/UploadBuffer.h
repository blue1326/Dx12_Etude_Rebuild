#ifndef UploadBuffer_h__
#define UploadBuffer_h__

#include "Component.h"
#include "BufferUtility.h"
#include "dxException.h"
template<typename T>
class UploadBuffer :public CComponent
{
public:
	explicit UploadBuffer(const shared_ptr<DxDevice> _device)
		:m_DxDevice(_device)
		,m_Heap(nullptr)
		,m_UploadBuffer(nullptr)
		,m_MappedData(nullptr)
		,m_ElementByteSize(0)
		, m_DescriptorSize(0)
	{}
	UploadBuffer(const UploadBuffer& rhs) = delete;
	UploadBuffer& operator=(const UploadBuffer&rhs) = delete;
	virtual ~UploadBuffer() 
	{
		if (m_UploadBuffer != nullptr)
			m_UploadBuffer->Unmap(0, nullptr);

		m_UploadBuffer = nullptr;
	}

private:
	ComPtr<ID3D12DescriptorHeap> m_Heap;
	ComPtr<ID3D12Resource> m_UploadBuffer;
	BYTE* m_MappedData;
	T data;
	UINT m_ElementByteSize;
	const shared_ptr<DxDevice> m_DxDevice;

	UINT m_DescriptorSize;

public:

	const D3D12_GPU_VIRTUAL_ADDRESS Address()
	{
		return m_UploadBuffer->GetGPUVirtualAddress();
	}

	const UINT Size()
	{
		return m_ElementByteSize;
	}
	void SetData(const T& _data)
	{
		data = _data;
		memcpy(&m_MappedData[0], &_data, sizeof(T));
	}
	ComPtr<ID3D12DescriptorHeap> GetHeap()
	{
		return m_Heap;
	}

	ID3D12Resource* Resource()
	{
		return m_UploadBuffer.Get();
	}
	const UINT& GetDesciptorSize()
	{
		return m_DescriptorSize;
	}

public:
	virtual HRESULT Init_Component() 
	{ 
		// Get the increment size of a descriptor in this heap type.  This is hardware specific, 
		// so we have to query this information.
		m_DescriptorSize = m_DxDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


		BuildDescriptorHeap();
		CreateUploadBuffer();

	
		return S_OK; 
	}
	virtual void Update_Component(const std::shared_ptr<CTimer> t){}
	virtual std::shared_ptr<CComponent> Clone() { return shared_ptr<CComponent>(new UploadBuffer<T>(m_DxDevice)); }
	virtual void OnResize() {}
private:
	void BuildDescriptorHeap();
	void BuildConstantBuffer();
	void CreateUploadBuffer();
};

template<typename T>
void UploadBuffer<T>::CreateUploadBuffer()
{
	m_ElementByteSize = BufferUtility::CalcConstantBufferByteSize(sizeof(T));

	ThrowIfFailed(m_DxDevice->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_UploadBuffer)));

	ThrowIfFailed(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
}

template<typename T>
void UploadBuffer<T>::BuildConstantBuffer()
{

}

template<typename T>
void UploadBuffer<T>::BuildDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = {};
	HeapDesc.NumDescriptors = 1;
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HeapDesc.NodeMask = 0;
	ThrowIfFailed(m_DxDevice->GetDevice()->CreateDescriptorHeap(&HeapDesc,
		IID_PPV_ARGS(&m_Heap)));
}


#endif // UploadBuffer_h__
