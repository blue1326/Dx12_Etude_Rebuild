#ifndef DxDevice_h__
#define DxDevice_h__

#include "stdHeaders.h"


class DxDevice
{
public:
	DxDevice();
	~DxDevice();
public:
	HRESULT InitDxDevice();
	bool DeviceInitState();
	void SetDeviceInitState(bool isready);

public : //public getter and setter
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() {
		return m_d3dDevice;
	}
	Microsoft::WRL::ComPtr<IDXGIFactory4> GetDxgiFactory()
	{
		return m_dxgiFactory;
	}
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList()
	{
		return m_CommandList;
	}
public:
	void LogAdapters(DXGI_FORMAT format);
	void LogAdapterOutputs(IDXGIAdapter* adapter, DXGI_FORMAT format);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
public :
	void Render_Begin();
	void Render_End();

private:
	HRESULT InitDirect3D_Device();

	HRESULT InitDirect3D_Environment();

	void CreateCommandObjects();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();


private: ///private
	ID3D12Resource * CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;
	////testcode
	//void SetCurrentBackBuffer(int _int)
	//{
	//	m_CurrBackBuffer = _int;
	//}
	//int GetSwapChainBufferCnt()
	//{
	//	return SwapChainBufferCount;
	//}
	//int GetCurrentBackBuffer()
	//{
	//	return m_CurrBackBuffer;
	//}
	//Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain()
	//{
	//	return m_SwapChain;
	//}

public:
	void FlushCommandQueue();
	
	
private:
	bool m_Initialized = false;
	//HWND      m_hMainWnd = nullptr;

	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
	
	Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;


	bool      m_4xMsaaState;    //false 4X MSAA enabled
	UINT      m_4xMsaaQuality;      //0 quality level of 4X MSAA

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
	UINT64 m_CurrentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_DirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;
public:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue();
private://pipelines
	HRESULT BuildPipelines();
	void BuildDescriptorHeaps();
	void BuildConstantBuffers();
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildPSO();
	unordered_map<string, vector<D3D12_INPUT_ELEMENT_DESC>> m_InputLayout;
	std::unordered_map<std::string, ComPtr<ID3DBlob>> m_Shaders;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_PSOs;


private:

	static const int SwapChainBufferCount = 4;
	int m_CurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_SwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DsvHeap;

	D3D12_VIEWPORT m_ScreenViewport;
	D3D12_RECT m_ScissorRect;

	ComPtr<ID3D12RootSignature> m_RootSignature = nullptr;


	///
	UINT m_RtvDescriptorSize = 0;
	UINT m_DsvDescriptorSize = 0;
	UINT m_CbvSrvUavDescriptorSize = 0;

	// Derived class should set these in derived constructor to customize starting values.
	D3D_DRIVER_TYPE m_d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

public:
	D3D12_VIEWPORT GetScreenViewport()
	{
		return m_ScreenViewport;
	}
	D3D12_RECT GetScissorRect()
	{
		return m_ScissorRect;
	}
public:
	DXGI_FORMAT GetBackBufferFormat()
	{
		return m_BackBufferFormat;
	}
	DXGI_FORMAT GetDepthStencilBufferFormat()
	{
		return m_DepthStencilFormat;
	}

public:
	bool Get4xMsaaState()const;//to renderer?
	UINT Get4xMsaaQuality()const;
	void Set4xMsaaState(bool value);//to renderer?
	virtual void OnResize();//to renderer?

};

#endif // DxDevice_h__
