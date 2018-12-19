#ifndef Object_h__
#define Object_h__
#include "stdHeaders.h"
#include "Component.h"
#include "DxDevice.h"

class CGameObject :public std::enable_shared_from_this<CGameObject>
{
protected:
	explicit CGameObject(shared_ptr<DxDevice> _device);
	virtual ~CGameObject();
public:
	//CComponent
	float Get_ViewZ(void) { return 0; }
public:
	virtual HRESULT Init_GameObject(void)=0;
	virtual int Update_GameObject(const std::shared_ptr<CTimer> t)=0;
	virtual int LastUpdate_GameObject(const std::shared_ptr<CTimer> t)=0;
	virtual void Render_GameObject()=0;
	//virtual void Render_GameObject_Depth(const CTimer& t)=0;
	//void AddComponent(std::string ComponentTag,std::shared_ptr<CComponent> component);
	//void AddComponent(eCOMPONENTS comptag, std::shared_ptr<CComponent> component);
	virtual void OnResize()PURE;

protected:
	virtual void Free(void);
	shared_ptr<DxDevice> m_DxDevice;

protected:
	void BuildDescriptorHeaps();
	void BuildConstantBuffers();
	ComPtr<ID3D12DescriptorHeap> m_CbvHeap;
protected:
	ComPtr<ID3D12Resource> m_UploadBuffer;
	BYTE* m_MappedData;

	UINT m_ElementByteSize = 0;
	void CreateUplaodBuffer();
};


#endif // Object_h__
