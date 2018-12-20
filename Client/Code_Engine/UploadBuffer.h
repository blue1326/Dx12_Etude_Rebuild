#ifndef UploadBuffer_h__
#define UploadBuffer_h__

#include "Component.h"
#include "BufferUtility.h"
#include "dxException.h"
template<typename T>
class CDiscriptor :public CComponent
{
public:
	explicit CDiscriptor(const shared_ptr<DxDevice> _device)
		:m_DxDevice(_device)
		,m_Heap(nullptr)
		,m_UploadBuffer(nullptr)
		,m_MappedData(NULL)
		,m_ElementByteSize(0)
	{}
	/*CDiscriptor(const CDiscriptor& rhs) = delete;
	CDiscriptor& operator=(const CDiscriptor&rhs) = delete;*/
	virtual ~CDiscriptor() {}

private:
	ComPtr<ID3D12DescriptorHeap> m_Heap;
	ComPtr<ID3D12Resource> m_UploadBuffer;
	BYTE* m_MappedData;
	T data;
	UINT m_ElementByteSize;
	const shared_ptr<DxDevice> m_DxDevice;
public:
	void SetData(const T& _data)
	{
		data = _data;
		memcpy(&m_MappedData[m_ElementByteSize], &data, sizeof(T));
	}
	


public:
	virtual HRESULT Init_Component() 
	{ 
		BuildDescriptorHeap();
		CreateUploadBuffer();
		return S_OK; 
	}
	virtual void Update_Component(const std::shared_ptr<CTimer> t){}
	virtual std::shared_ptr<CComponent> Clone() { return shared_ptr<CComponent>(new CDiscriptor<T>(m_DxDevice)); }
	virtual void OnResize() {}
private:
	void BuildDescriptorHeap();
	void BuildConstantBuffer();
	void CreateUploadBuffer();
};

template<typename T>
void CDiscriptor<T>::CreateUploadBuffer()
{
	m_ElementByteSize = BufferUtility::CalcConstantBufferByteSize(sizeof(T));

	ThrowIfFailed(m_DxDevice->GetDevice().Get()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_UploadBuffer)));

	ThrowIfFailed(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
}

template<typename T>
void CDiscriptor<T>::BuildConstantBuffer()
{

}

template<typename T>
void CDiscriptor<T>::BuildDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = 1;
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HeapDesc.NodeMask = 0;
	ThrowIfFailed(m_DxDevice->GetDevice()->CreateDescriptorHeap(&HeapDesc,
		IID_PPV_ARGS(&m_Heap)));
}


#endif // UploadBuffer_h__
