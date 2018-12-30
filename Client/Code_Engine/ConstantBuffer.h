#ifndef ConstantBuffer_h__
#define ConstantBuffer_h__

#include "Component.h"
#include "BufferUtility.h"
#include "dxException.h"

template<typename T>
class CConstantBuffer :public CComponent
{
public:
	explicit CConstantBuffer(const shared_ptr<DxDevice> _device)
		:m_DxDevice(_device)
		,m_UploadBuffer(nullptr)
		, m_ElementByteSize(BufferUtility::CalcConstantBufferByteSize(sizeof(T)))
		,m_MappedData(nullptr)
	{}
	CConstantBuffer(const CConstantBuffer& rhs) = delete;
	CConstantBuffer& operator=(const CConstantBuffer&rhs) = delete;
	virtual ~CConstantBuffer() 
	{
		if (m_UploadBuffer != nullptr)
			m_UploadBuffer->Unmap(0, nullptr);

		m_UploadBuffer = nullptr;
	}

private:
	//ComPtr<ID3D12DescriptorHeap> m_Heap;
	ComPtr<ID3D12Resource> m_UploadBuffer;
	BYTE* m_MappedData;
	T data;
	UINT m_ElementByteSize;

	const shared_ptr<DxDevice> m_DxDevice;

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
		int tmp = sizeof(T);
		
		//memcpy_s(m_MappedData, &data, sizeof(T));
		int i = memcpy_s(m_MappedData, m_ElementByteSize, &data, sizeof(T));
	}
	const T GetData()
	{
		T data;
		memcpy_s(&data, sizeof(T), m_MappedData, sizeof(T));
		return data;
	}
	ID3D12Resource* Resource()
	{
		return m_UploadBuffer.Get();
	}


public:
	virtual HRESULT Init_Component() 
	{ 
		CreateUploadBuffer();
		return S_OK; 
	}
	virtual void Update_Component(const std::shared_ptr<CTimer> t){}
	virtual std::shared_ptr<CComponent> Clone() { return shared_ptr<CComponent>(new CConstantBuffer<T>(m_DxDevice)); }
	virtual void OnResize() {}
private:
	void CreateUploadBuffer();
};

template<typename T>
void CConstantBuffer<T>::CreateUploadBuffer()
{
	ThrowIfFailed(m_DxDevice->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_UploadBuffer)));

	ThrowIfFailed(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
}


#endif // UploadBuffer_h__
