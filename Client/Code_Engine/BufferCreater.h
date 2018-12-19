#ifndef BufferCreater_h__
#define BufferCreater_h__
#include "stdHeaders.h"
class BufferCreater
{
public:
	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);
};



#endif // BufferCreater_h__
