#include "TestObject3.h"
#include "ComponentHolder.h"

#include "BasicMesh_Crate.h"
//#include "box.h"
#include "Transform.h"
#include "Renderer.h"
#include "ConstantBuffer.h"
//test
#include "dxException.h"
CTestObject3::CTestObject3(shared_ptr<DxDevice> _device)
	: CGameObject::CGameObject(_device)
	,pBox(nullptr)
	, pTransform(nullptr)
	, pRenderer(nullptr)
{

}

CTestObject3::~CTestObject3()
{

}

HRESULT CTestObject3::Init_GameObject(void)
{
	pTransform = CComponentHolder::GetInstance()->Clone_Component("Transform");
	((CTransform*)pTransform.get())->Init_Component();
	((CTransform*)pTransform.get())->SetPosition(-1, 0, 2);
	pBox = CComponentHolder::GetInstance()->Clone_Component("Basic_Crate");
	pBox->Init_Component();
	pRenderer = CComponentHolder::GetInstance()->Clone_Component("Renderer");
	
	pConstant = CComponentHolder::GetInstance()->Clone_Component("testConstant");
	pConstant->Init_Component();


	return S_OK;
}

int CTestObject3::Update_GameObject(const std::shared_ptr<CTimer> t)
{
	((CTransform*)pTransform.get())->Update_Component(t);


	XMMATRIX world, WVP;
	world = ((CTransform*)pTransform.get())->GetWorld();
	WVP = world * m_DxDevice->GetViewMatrix() * m_DxDevice->GetProjMatrix();
		

	ObjectConstant objConstants;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(WVP));
	dynamic_cast<CConstantBuffer<ObjectConstant>*>(pConstant.get())->SetData(objConstants);



	int emementidx = 0; //it is samplecode
	//copydata
	//memcpy(&m_MappedData[emementidx * m_ElementByteSize], &objConstants, sizeof(objConstants));

	//dynamic_cast<CBox*>(pBox.get())->Update_Component(t);
	dynamic_cast<CRenderer*>(pRenderer.get())->Add_RenderList(CRenderer::RENDER_DEBUG_TEST, this->shared_from_this());
	//((CRenderer*)pRenderer.get())->Add_RenderList(CRenderer::RENDER_NONEALPHA, this->shared_from_this());
	
	return 0;
}

int CTestObject3::LastUpdate_GameObject(const std::shared_ptr<CTimer> t)
{
	return 0;
}

void CTestObject3::Render_GameObject()
{
	ComPtr<ID3D12Device> device = m_DxDevice->GetDevice();
	ComPtr<ID3D12GraphicsCommandList> cmdList = m_DxDevice->GetCommandList();

	
	//ID3D12DescriptorHeap* descriptorHeaps[] = { dynamic_cast<CConstantBuffer<ObjectConstant>*>(pConstant.get())->GetHeap().Get() };
	//cmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	cmdList->SetGraphicsRootSignature(dynamic_cast<CRenderer*>(pRenderer.get())->GetRootSignature("Debug_test").Get());

	cmdList->IASetVertexBuffers(0, 1, &(dynamic_cast<CBasicMesh_Crate*>(pBox.get())->GetGeometry()->VertexBufferView()));
	cmdList->IASetIndexBuffer(&(dynamic_cast<CBasicMesh_Crate*>(pBox.get())->GetGeometry()->IndexBufferView()));
	cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = dynamic_cast<CConstantBuffer<ObjectConstant>*>(pConstant.get())->Resource()->GetGPUVirtualAddress();
	cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);
	
	//cmdList->SetGraphicsRootDescriptorTable(0, dynamic_cast<CDiscriptor<ObjectConstant>*>(pConstant.get())->GetHeap()->GetGPUDescriptorHandleForHeapStart());
	cmdList->DrawIndexedInstanced(dynamic_cast<CBasicMesh_Crate*>(pBox.get())->GetGeometry()->DrawArg.IndexCount,
		1, 0, 0, 0);

}

void CTestObject3::OnResize()
{

}

