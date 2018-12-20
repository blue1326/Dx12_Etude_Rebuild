#include "TestObject.h"
#include "ComponentHolder.h"
#include "box.h"
#include "Transform.h"
#include "Renderer.h"

//test
#include "dxException.h"
CTestObject::CTestObject(shared_ptr<DxDevice> _device)
	: CGameObject::CGameObject(_device)
	,pBox(nullptr)
	, pTransform(nullptr)
	, pRenderer(nullptr)
{

}

CTestObject::~CTestObject()
{

}

HRESULT CTestObject::Init_GameObject(void)
{
	pTransform = CComponentHolder::GetInstance()->Clone_Component("Transform");
	((CTransform*)pTransform.get())->Init_Component();
	((CTransform*)pTransform.get())->SetPosition(0, 0, 0);
	pBox = CComponentHolder::GetInstance()->Clone_Component("Box");
	((CBox*)pBox.get())->Init_Component();
	pRenderer = CComponentHolder::GetInstance()->Clone_Component("Renderer");
	
	BuildDescriptorHeaps();
	BuildConstantBuffers();


	return S_OK;
}

int CTestObject::Update_GameObject(const std::shared_ptr<CTimer> t)
{
	((CTransform*)pTransform.get())->Update_Component(t);


	XMMATRIX world, WVP;
	world = ((CTransform*)pTransform.get())->GetWorld();
	WVP = world * m_DxDevice->GetViewMatrix() * m_DxDevice->GetProjMatrix();
		

	ObjectConstants objConstants;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(WVP));


	int emementidx = 0; //it is samplecode
	//copydata
	memcpy(&m_MappedData[emementidx * m_ElementByteSize], &objConstants, sizeof(objConstants));

	//dynamic_cast<CBox*>(pBox.get())->Update_Component(t);
	dynamic_cast<CRenderer*>(pRenderer.get())->Add_RenderList(CRenderer::RENDER_DEBUG, this->shared_from_this());
	//((CRenderer*)pRenderer.get())->Add_RenderList(CRenderer::RENDER_NONEALPHA, this->shared_from_this());
	
	return 0;
}

int CTestObject::LastUpdate_GameObject(const std::shared_ptr<CTimer> t)
{
	return 0;
}

void CTestObject::Render_GameObject()
{
	ComPtr<ID3D12Device> device = m_DxDevice->GetDevice();
	ComPtr<ID3D12GraphicsCommandList> cmdList = m_DxDevice->GetCommandList();

	
	//cmdList->SetPipelineState(dynamic_cast<CRenderer*>(pRenderer.get())->GetPSO("opaquePSO").Get());

	//ID3D12DescriptorHeap* descriptorHeaps[] = { m_CbvHeap.Get() };
	//ID3D12DescriptorHeap* descriptorHeaps[] = { dynamic_cast<CBox*>(pBox.get())->GetCBVHeap().Get() };
	ID3D12DescriptorHeap* descriptorHeaps[] = {m_CbvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	cmdList->SetGraphicsRootSignature(dynamic_cast<CRenderer*>(pRenderer.get())->GetRootSignature("Debug").Get());
	cmdList->IASetVertexBuffers(0, 1, &(dynamic_cast<CBox*>(pBox.get())->GetGeometry()->VertexBufferView()));
	cmdList->IASetIndexBuffer(&(dynamic_cast<CBox*>(pBox.get())->GetGeometry()->IndexBufferView()));
	cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->SetGraphicsRootDescriptorTable(0, m_CbvHeap->GetGPUDescriptorHandleForHeapStart());
	cmdList->DrawIndexedInstanced(dynamic_cast<CBox*>(pBox.get())->GetGeometry()->DrawArgs["box"].IndexCount,
		1, 0, 0, 0);

}

void CTestObject::OnResize()
{

}

