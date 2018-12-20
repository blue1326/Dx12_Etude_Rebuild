#ifndef Renderer_h__
#define Renderer_h__
#include "Component.h"
#include "stdHeaders.h"
//#include "DefinedHeaders.h"
#include "DxDevice.h"
//#include "UploadBuffer.h"
#include "GameObject.h"
#include "Timer.h"



class CRenderer : public CComponent
{
public:
	enum RENDER { RENDER_PRIORITY, RENDER_NONEALPHA, RENDER_ALPHA, RENDER_UI, RENDER_DEBUG, RENDER_DUAL, RENDER_END };
public:
	explicit CRenderer(const shared_ptr<DxDevice> _device);
	virtual ~CRenderer();
public:
	virtual HRESULT Init_Component()override;
	virtual void Update_Component(const std::shared_ptr<CTimer> t)override;
	// render to gameobject
	virtual void Render_GameObject(void);

	void RenderNoneAlpha(void);
	void RenderDebug(void);

	HRESULT Add_RenderList(RENDER eType, shared_ptr<CGameObject> object);

public:
	virtual std::shared_ptr<CComponent> Clone()override;

	
private://render list
private:
	list<shared_ptr<CGameObject>>				m_RenderList[RENDER_END];
	typedef list<shared_ptr<CGameObject>>		RENDERLIST;
public:
	void Clear_RenderList();

private:
	const shared_ptr<DxDevice> m_DxDevice;

private:
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildPSOs();

	void BuildRootSignature_Debug();
	void BuildShadersAndInputLayout_Debug();
	void BuildRootSignature_Default();
	void BuildShadersAndInputLayout_Default();
	void BuildPSO_Debug();
	void BuildPSO_Default();

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
private://rootsignatures
	//ComPtr<ID3D12RootSignature> m_RootSignature;

	typedef unordered_map<string, ComPtr<ID3D12RootSignature>> ROOTSIGNATURE;
	ROOTSIGNATURE m_RootSignatures;
private://shader
	ComPtr<ID3DBlob> m_vsByteCode;
	ComPtr<ID3DBlob> m_psByteCode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
	typedef unordered_map<string, vector<D3D12_INPUT_ELEMENT_DESC>> INPUTLAYOUT;
	INPUTLAYOUT m_InputLayouts;

private://pso
	ComPtr<ID3D12PipelineState> m_PSO;
	typedef unordered_map < string, ComPtr<ID3D12PipelineState>> MAP_PSOS;
	MAP_PSOS m_PSOs;

	typedef unordered_map<string, ComPtr<ID3DBlob>> SHADERS;
	SHADERS m_Shaders;

public:
	ComPtr<ID3D12PipelineState> GetPSO()
	{
		return m_PSO;
	}
	ComPtr<ID3D12PipelineState> GetPSO(string _str)
	{
		return m_PSOs[_str];
	}
	ComPtr<ID3D12RootSignature> GetRootSignature(string _str)
	{
		return m_RootSignatures[_str];
	}
};
#endif // Renderer_h__
