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
	enum RENDER { RENDER_PRIORITY, RENDER_NONEALPHA, RENDER_ALPHA, RENDER_UI, RENDER_DUAL, RENDER_END };
public:
	explicit CRenderer(const shared_ptr<DxDevice> _device);
	virtual ~CRenderer();
public:
	virtual HRESULT Init_Component()override;
	virtual void Update_Component(const std::shared_ptr<CTimer> t)override;
	// render to gameobject
	virtual void Render_GameObject(void);

	void RenderNoneAlpha(void);

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
private://rootsignature
	ComPtr<ID3D12RootSignature> m_RootSignature;
private://shader
	ComPtr<ID3DBlob> m_vsByteCode;
	ComPtr<ID3DBlob> m_psByteCode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
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
	ComPtr<ID3D12RootSignature> GetRootSignature()
	{
		return m_RootSignature;
	}
};
#endif // Renderer_h__
