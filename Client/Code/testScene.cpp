#include "testScene.h"
#include "Layer.h"
#include "TestObject.h"
#include "TestObject2.h"
#include "TestObject_Camera.h"


CScene_Test::CScene_Test(shared_ptr<DxDevice> _device)
	:CScene::CScene(_device)
{

}

CScene_Test::~CScene_Test()
{

}

HRESULT CScene_Test::Ready_Scene()
{
	if (FAILED(Ready_LayerTestCam(L"LayerTestCam")))
		return E_FAIL;
	if (FAILED(Ready_LayerTest(L"LayerTest")))
		return E_FAIL;

	return S_OK;
}

int CScene_Test::Update_Scene(const std::shared_ptr<CTimer> t)
{
	return CScene::Update_Scene(t);
}

int CScene_Test::LastUpdate_Scene(const std::shared_ptr<CTimer> t)
{
	return CScene::LastUpdate_Scene(t);
}

void CScene_Test::Render_Scene(void)
{

}

HRESULT CScene_Test::Ready_LayerTestCam(const wchar_t* pLayerTag)
{
	shared_ptr<CLayer> Layer = shared_ptr<CLayer>(new CLayer);
	if (nullptr == Layer)
		return E_FAIL;

	Layer->Add_GameObject(shared_ptr<CGameObject>(new CTestObject_Camera(m_DxDevice)));
	
	
	
	if (FAILED(Layer->Ready_Layer()))
		return E_FAIL;


	CScene::Add_Layer(pLayerTag, Layer);
	return S_OK;
}

HRESULT CScene_Test::Ready_LayerTest(const wchar_t* pLayerTag)
{
	shared_ptr<CLayer> Layer = shared_ptr<CLayer>(new CLayer);
	if (nullptr == Layer)
		return E_FAIL;

	Layer->Add_GameObject(shared_ptr<CGameObject>(new CTestObject(m_DxDevice)));
	Layer->Add_GameObject(shared_ptr<CGameObject>(new CTestObject2(m_DxDevice)));

	if (FAILED(Layer->Ready_Layer()))
		return E_FAIL;



	CScene::Add_Layer(pLayerTag, Layer);
	return S_OK;

}

