#ifndef UploadBuffer_h__
#define UploadBuffer_h__

#include "Component.h"
#include "BufferUtility.h"
#include "dxException.h"
template<typename T>
class CUploadBuffer :public CComponent
{
public:
	explicit CUploadBuffer(const shared_ptr<DxDevice> _device)
		:m_DxDevice(_device)
		,m_Heap(nullptr)
		,m_UploadBuffer(nullptr)
		,m_MappedData(NULL)
		,m_ElementByteSize(0)
	{}
	virtual ~CUploadBuffer() {}

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
	}
	


public:
	virtual HRESULT Init_Component() { return S_OK; }
	virtual void Update_Component(const std::shared_ptr<CTimer> t){}
	virtual std::shared_ptr<CComponent> Clone() { return nullptr; }
	virtual void OnResize() {}
private:
	void BuildDescriptorHeap();
	void BuildConstantBuffer();
	void CreateUploadBuffer();
};

template<typename T>
void CUploadBuffer<T>::CreateUploadBuffer()
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
void CUploadBuffer<T>::BuildConstantBuffer()
{

}

template<typename T>
void CUploadBuffer<T>::BuildDescriptorHeap()
{

}


#endif // UploadBuffer_h__
