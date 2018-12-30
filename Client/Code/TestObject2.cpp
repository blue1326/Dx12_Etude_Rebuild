#include "TestObject2.h"
#include "ComponentHolder.h"
#include "box.h"
#include "Transform.h"
#include "Renderer.h"
#include "BasicMesh_Crate.h"
#include "DescriptorHeap.h"
#include "ConstantBuffer.h"
#include "Material.h"
//test
//#include "dxException.h"
CTestObject2::CTestObject2(shared_ptr<DxDevice> _device)
	: CGameObject::CGameObject(_device)
	,pBox(nullptr)
	, pTransform(nullptr)
	, pRenderer(nullptr)
	, pCrate(nullptr)
{

}

CTestObject2::~CTestObject2()
{

}

HRESULT CTestObject2::Init_GameObject(void)
{
	pTransform = CComponentHolder::GetInstance()->Clone_Component("Transform");
	((CTransform*)pTransform.get())->Init_Component();
	((CTransform*)pTransform.get())->SetPosition(0, 0, 0);
	//((CTransform*)pTransform.get())->SetScale(1);
	pBox = CComponentHolder::GetInstance()->Clone_Component("Box");
	((CBox*)pBox.get())->Init_Component();
	pRenderer = CComponentHolder::GetInstance()->Clone_Component("Renderer");
	
	pCrate = CComponentHolder::GetInstance()->Clone_Component("Basic_Crate");

	pDescriptorHeap = CComponentHolder::GetInstance()->Clone_Component("DescriptorHeap");
	pDescriptorHeap->Init_Component();

	pMaterial = CComponentHolder::GetInstance()->Clone_Component("Tex_Crate");
	pMaterial->Init_Component();

	dynamic_cast<CMaterial*>(pMaterial.get())->SetUpDescripterHeap(dynamic_cast<CDescriptorHeap*>(pDescriptorHeap.get())->GetHeap());

	//CBs
	pObjConstant = CComponentHolder::GetInstance()->Clone_Component("ObjConstant");
	pObjConstant->Init_Component();
	pPassConstant = CComponentHolder::GetInstance()->Clone_Component("PassConstant");
	pPassConstant->Init_Component();
	pMatConstant = CComponentHolder::GetInstance()->Clone_Component("MatConstant");
	pMatConstant->Init_Component();

	
	/*BuildDescriptorHeaps();
	BuildConstantBuffers();*/


	return S_OK;
}

int CTestObject2::Update_GameObject(const std::shared_ptr<CTimer> t)
{
	pTransform->Update_Component(t);



	XMMATRIX world = dynamic_cast<CTransform*>(pTransform.get())->GetWorld();
	//passconst
	PassConstants tmp = m_DxDevice->GetPassConstant();
	dynamic_cast<CConstantBuffer<PassConstants>*>(pPassConstant.get())->SetData(tmp);

	//objconst
	ObjectConstants _OC;

	XMStoreFloat4x4(&_OC.World, XMMatrixTranspose(world));
	XMStoreFloat4x4(&_OC.TexTransform,XMMatrixTranspose(XMMatrixIdentity()));
	dynamic_cast<CConstantBuffer<ObjectConstants>*>(pObjConstant.get())->SetData(_OC);


	ObjectConstants _OCtmp =dynamic_cast<CConstantBuffer<ObjectConstants>*>(pObjConstant.get())->GetData();
	



	//matconst
	Material tmpmaterial = *dynamic_cast<CMaterial*>(pMaterial.get())->GetMaterial().get();
	XMMATRIX matTransform = XMLoadFloat4x4(&tmpmaterial.MatTransform);
	MaterialConstants _MC;
	_MC.DiffuseAlbedo = tmpmaterial.DiffuseAlbedo;
	_MC.FresnelR0 = tmpmaterial.FresnelR0;
	_MC.Roughness = tmpmaterial.Roughness;
	XMStoreFloat4x4(&_MC.MatTransform, XMMatrixTranspose(matTransform));

	dynamic_cast<CConstantBuffer<MaterialConstants>*>(pMatConstant.get())->SetData(_MC);

	MaterialConstants _MCtmp = dynamic_cast<CConstantBuffer<MaterialConstants>*>(pMatConstant.get())->GetData();



	//dynamic_cast<CBox*>(pBox.get())->Update_Component(t);
	dynamic_cast<CRenderer*>(pRenderer.get())->Add_RenderList(CRenderer::RENDER_NONEALPHA, this->shared_from_this());
	//((CRenderer*)pRenderer.get())->Add_RenderList(CRenderer::RENDER_NONEALPHA, this->shared_from_this());
	
	return 0;
}

int CTestObject2::LastUpdate_GameObject(const std::shared_ptr<CTimer> t)
{
	return 0;
}

void CTestObject2::Render_GameObject()
{
	ComPtr<ID3D12Device> device = m_DxDevice->GetDevice();
	ComPtr<ID3D12GraphicsCommandList> cmdList = m_DxDevice->GetCommandList();

	
	ID3D12DescriptorHeap* descriptorHeaps[] = { dynamic_cast<CDescriptorHeap*>(pDescriptorHeap.get())->GetHeap().Get() };
	cmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	cmdList->SetGraphicsRootSignature(dynamic_cast<CRenderer*>(pRenderer.get())->GetRootSignature("Default").Get());

	cmdList->IASetVertexBuffers(0, 1, &(dynamic_cast<CBasicMesh_Crate*>(pCrate.get())->GetGeometry()->VertexBufferView()));
	cmdList->IASetIndexBuffer(&(dynamic_cast<CBasicMesh_Crate*>(pCrate.get())->GetGeometry()->IndexBufferView()));
	cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(dynamic_cast<CDescriptorHeap*>(pDescriptorHeap.get())->GetHeap()->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(0, dynamic_cast<CDescriptorHeap*>(pDescriptorHeap.get())->GetDescriptorSize());

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = dynamic_cast<CConstantBuffer<ObjectConstants>*>(pObjConstant.get())->Address();
	D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = dynamic_cast<CConstantBuffer<MaterialConstants>*>(pMatConstant.get())->Address();
	D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = dynamic_cast<CConstantBuffer<PassConstants>*>(pPassConstant.get())->Address();

	cmdList->SetGraphicsRootDescriptorTable(0, tex);
	cmdList->SetGraphicsRootConstantBufferView(1, objCBAddress);
	cmdList->SetGraphicsRootConstantBufferView(2, passCBAddress);
	cmdList->SetGraphicsRootConstantBufferView(3, matCBAddress);
	
	cmdList->DrawIndexedInstanced(dynamic_cast<CBasicMesh_Crate*>(pCrate.get())->GetGeometry()->DrawArg.IndexCount,
		1, 0, 0, 0);

}

void CTestObject2::OnResize()
{

}

