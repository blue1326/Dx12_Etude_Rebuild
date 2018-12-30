#ifndef DescriptorHeap_h__
#define DescriptorHeap_h__
#include "Component.h"
#include "DxDevice.h"
class CDescriptorHeap : public CComponent
{
public:
	explicit CDescriptorHeap(const shared_ptr<DxDevice> _device);
	virtual ~CDescriptorHeap();
	CDescriptorHeap(const CDescriptorHeap& rhs) = delete;
	CDescriptorHeap& operator=(const CDescriptorHeap&rhs) = delete;
public:
	virtual HRESULT Init_Component();
	virtual void Update_Component();
	virtual shared_ptr<CComponent> Clone();
	virtual void OnResize();
private:
	void BuildDescriptorHeap();

private:
	const shared_ptr<DxDevice> m_DxDevice;

	UINT m_DescriptorSize;

	ComPtr<ID3D12DescriptorHeap> m_Heap;

	D3D12_DESCRIPTOR_HEAP_DESC m_HeapDesc;
	bool isDescSetup;
public:
	const UINT GetDescriptorSize()
	{
		return m_DescriptorSize;
	}
	void SetUpHeapDesc(D3D12_DESCRIPTOR_HEAP_DESC& _desc);
	ComPtr<ID3D12DescriptorHeap> GetHeap()
	{
		return m_Heap;
	}
};


#endif // DescriptorHeap_h__
