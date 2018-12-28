//#include "stdHeaders.h"
#include "GeneralSettings.h"
//#include "Renderer.h"

#include "Component.h"
#include "ComponentHolder.h"
//#include "StageFirst.h"
//#include "ComponentHolder.h"
#include "testScene.h"

#include "Transform.h"
#include "Camera.h"
#include "Renderer.h" //나중에 대체
#include "box.h"
#include "BasicMesh_Crate.h"
#include "Material.h"
#include "Controller.h"
#include "UploadBuffer.h"
#include "DescriptorHeap.h"

CGeneralSettings::CGeneralSettings(const std::shared_ptr<DxDevice> Device, const std::shared_ptr<CTimer> Timer1, const std::shared_ptr<CTimer> Timer2)
	: m_DxDevice(Device), m_Timer1(move(Timer1)), m_Timer2(move(Timer2)),m_fTimeAcc(0.f),m_CallPerSec(0.f)
{
	m_Timer1->Reset();
	m_Timer2->Reset();
}

CGeneralSettings::~CGeneralSettings()
{
	
}

bool CGeneralSettings::InitGeneralSetting()
{
	if (FAILED(InitDxDevice()))
		return false;
	g_DeviceInitState = true;
	if (FAILED(InitComponents()))
		return false;
	if (FAILED(InitScene()))
		return false;
	
	return true;
}

HRESULT CGeneralSettings::InitScene()
{
	/*m_Scene = std::shared_ptr<CScene>(new CStageFirst);
	if (!m_Scene->Ready_Scene())
		return false;
	m_Scene->SetActiveState(true);*/
	m_Scene = std::shared_ptr<CScene>(new CScene_Test(m_DxDevice));
	if (FAILED(m_Scene->Ready_Scene()))
		return E_FAIL;
	m_Scene->SetActiveState(true);

	return S_OK;
}

HRESULT CGeneralSettings::InitComponents()
{
	CComponentHolder::GetInstance()->Reserve_ComponentHolder(20);

	shared_ptr<CComponent> inst = std::shared_ptr<CComponent>(new CRenderer(m_DxDevice));
	if(FAILED(inst->Init_Component()))
		return E_FAIL;
	CComponentHolder::GetInstance()->AddOriginComponent("Renderer", inst);

	inst.reset(new CController);
	m_Controller = inst;
	CComponentHolder::GetInstance()->AddOriginComponent("Controller", inst);

	inst.reset(new CTransform(m_DxDevice));
	CComponentHolder::GetInstance()->AddOriginComponent("Transform", inst);

	inst.reset(new CCamera(m_DxDevice));
	CComponentHolder::GetInstance()->AddOriginComponent("Camera", inst);

	inst.reset(new UploadBuffer<ObjectConstants>(m_DxDevice));
	CComponentHolder::GetInstance()->AddOriginComponent("ObjConstant", inst);

	inst.reset(new UploadBuffer<MaterialConstants>(m_DxDevice));
	CComponentHolder::GetInstance()->AddOriginComponent("MatConstant", inst);

	inst.reset(new UploadBuffer<PassConstants>(m_DxDevice));
	CComponentHolder::GetInstance()->AddOriginComponent("PassConstant", inst);
	
	inst.reset(new UploadBuffer<ObjectConstant>(m_DxDevice));
	CComponentHolder::GetInstance()->AddOriginComponent("testConstant", inst);

	inst.reset(new CDescriptorHeap(m_DxDevice));
	CComponentHolder::GetInstance()->AddOriginComponent("DescriptorHeap", inst);

	inst.reset(new CBox(m_DxDevice));
	if (FAILED(inst->Init_Component()))
		return E_FAIL;
	CComponentHolder::GetInstance()->AddOriginComponent("Box", inst);

	inst.reset(new CBasicMesh_Crate(m_DxDevice));
	if (FAILED(inst->Init_Component()))
		return E_FAIL;
	CComponentHolder::GetInstance()->AddOriginComponent("Basic_Crate", inst);

	inst.reset(new CMaterial(m_DxDevice));
	auto woodCrate = make_shared<Material>();
	woodCrate->Name = "woodCrate";
	woodCrate->MatCBIndex = 0;
	woodCrate->DiffuseSrvHeapIndex = 0;
	woodCrate->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	woodCrate->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	woodCrate->Roughness = 0.2f;
	dynamic_cast<CMaterial*>(inst.get())->SetUpMaterialState(woodCrate);
	dynamic_cast<CMaterial*>(inst.get())->SetUpTextureLocation(L"../../Textures/WoodCrate02.dds");
	if (FAILED(inst->Init_Component()))
		return E_FAIL;
	CComponentHolder::GetInstance()->AddOriginComponent("Tex_Crate", inst);
	return S_OK;
}

///////////////////////////////////////////////////////////for Generic Method
void CGeneralSettings::Update()
{
	m_Timer2->Tick();
	m_Scene->Update_Scene(m_Timer2);

	//RECT rc;
	//POINT p1, p2;
	//GetClientRect(g_hWnd, &rc);

	//p1.x = rc.left;
	//p1.y = rc.top;
	//p2.x = rc.right;
	//p2.y = rc.bottom;

	//ClientToScreen(g_hWnd, &p1);
	//ClientToScreen(g_hWnd, &p2);
	//rc.left = p1.x;
	//rc.top = p1.y;
	//rc.right = p2.x;
	//rc.bottom = p2.y;
	//ClipCursor(&rc);
	
	
}

void CGeneralSettings::Last_Update()
{

	m_Scene->LastUpdate_Scene(m_Timer2);
	//system("cls");
	
}

void CGeneralSettings::Render()
{
	
	
		m_DxDevice->Render_Begin();

		dynamic_cast<CRenderer*>(CComponentHolder::GetInstance()->Get_Component("Renderer").get())->Render_GameObject();

		m_DxDevice->Render_End();


}
//
//void CGeneralSettings::SetClientSize(int width, int height) {
//	
//	((CRenderer*)CComponentHolder::GetInstance()->Get_Component("Renderer").get())->SetClientSize(width, height);
//}

void CGeneralSettings::OnResize()
{
	if(g_DeviceInitState)
		m_DxDevice->OnResize();
	m_Scene->OnResize();

	
}
//////////////////////////////////////////////////////////for TimerMethod
std::shared_ptr<CTimer> CGeneralSettings::GetTimer(TimerDef td)
{
	switch (td)
	{
	case MainTimer:
		return m_Timer1;
	case SubTimer:
		return m_Timer2;
	case AllTimer:
		if (m_Timer1 != nullptr && m_Timer2 != nullptr)
			return m_Timer1;
		else
			return nullptr;
	}
	return nullptr;
}

void CGeneralSettings::TimerReset(TimerDef td)
{
	switch (td)
	{
	case MainTimer:
		return m_Timer1->Reset();
	case SubTimer:
		return m_Timer2->Reset();
	case AllTimer:
		m_Timer1->Reset();
		m_Timer2->Reset();
	}
	
}

void CGeneralSettings::TimerStart(TimerDef td)
{
	switch (td)
	{
	case MainTimer:
		m_Timer1->Start();
		return;
	case SubTimer:
		m_Timer2->Start();
		return;
	case AllTimer:
		m_Timer1->Start();
		m_Timer2->Start();
		return;
	}
	
}

void CGeneralSettings::TimerStop(TimerDef td)
{
	switch (td)
	{
	case MainTimer:
		m_Timer1->Stop();
		return;
	case SubTimer:
		m_Timer2->Stop();
		return;
	case AllTimer:
		m_Timer1->Stop();
		m_Timer2->Stop();
		return;
	}
	
}

void CGeneralSettings::TimerTick(TimerDef td)
{
	switch (td)
	{
	case MainTimer:
		m_Timer1->Tick();
		return;
	case SubTimer:
		m_Timer2->Tick();
		return; 
	case AllTimer:
		m_Timer1->Tick();
		m_Timer2->Tick();
		return;
	}
	
}


void CGeneralSettings::OnMouseMove(WPARAM btnState, int x, int y)
{
	CController* pController = dynamic_cast<CController*>(m_Controller.get());
	mouse_x = x;
	mouse_y = y;
	pController->Update_MouseXY(x, y);
	pController->Update_MouseBtnState(btnState);

}

void CGeneralSettings::SetFramelateLimit(const float& _Limit)
{
	m_CallPerSec = 1.f / _Limit;
}



std::shared_ptr<DxDevice> CGeneralSettings::GetDevice()
{
	return m_DxDevice;
}

HRESULT CGeneralSettings::InitDxDevice()
{
	if (FAILED(m_DxDevice->InitDxDevice()))
		return E_FAIL;
	g_DeviceInitState = true;
	return S_OK;
}


bool CGeneralSettings::CalculateFrameStats(const HWND& mainWnd, std::wstring caption)
{
	
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	bool isLimit = false;
	static int frameCnt = 0;
	static int frameCntLimit = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;
	

	m_fTimeAcc += m_Timer1->DeltaTime();
	if (m_fTimeAcc > m_CallPerSec)
	{
		frameCntLimit++;
		m_fTimeAcc = 0.f;
		isLimit = true;
	}

	
	// Compute averages over one second period.
	if ((m_Timer1->TotalTime() - timeElapsed) >= 1.0f)
	{
		int fps = frameCnt; // fps = frameCnt / 1
		
		int limitedfps = frameCntLimit;
		
		float mspf = 1000.0f / fps;

		std::wstring limitfpsStr = std::to_wstring(limitedfps);
		std::wstring fpsStr = std::to_wstring(fps);
		std::wstring mspfStr = std::to_wstring(mspf);
		
		///testcode
		wstring mousex = to_wstring(mouse_x);
		wstring mousey = to_wstring(mouse_y);


		std::wstring windowText = caption +
			L" Limited Fps: " + limitfpsStr +
			L"  Fps: " + fpsStr +
			L"  mspf: " + mspfStr +
			L"  mx: " + mousex +
			L"  my: " + mousey;

		SetWindowText(mainWnd, windowText.c_str());

		// Reset for next average.
		frameCnt = 0;
		frameCntLimit = 0;
		timeElapsed += 1.0f;
	}

	return isLimit;
}

