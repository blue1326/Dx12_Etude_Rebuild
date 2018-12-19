#include "API_Setup.h"



//HWND g_hWnd;
//HINSTANCE g_hInst;
//int g_ClientWidth = 800;
//int g_ClientHeight = 600;
//bool g_DeviceInitState = false;
//XMMATRIX g_matView = XMMatrixIdentity();
//XMMATRIX g_matProj = XMMatrixIdentity();

CApiSetUP* CApiSetUP::m_App = nullptr; //static initalize

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return CApiSetUP::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
}




CApiSetUP::CApiSetUP(HINSTANCE hInstance)
	:m_hAppInst(hInstance)
{
	assert(m_App == nullptr); //running on debug state
	m_App = this;
}

CApiSetUP::CApiSetUP()
{

}

CApiSetUP::~CApiSetUP()
{
	//not yet using...
}

CApiSetUP* CApiSetUP::GetApp()
{
	return m_App;
}

HINSTANCE CApiSetUP::AppInst() const
{
	return m_hAppInst;
}

HWND CApiSetUP::MainWnd() const
{
	return m_hMainWnd;
}

int CApiSetUP::Run()
{
	MSG msg = { 0 };
	//assert(m_GS != nullptr);
	//need timer
	//assert(m_GS->GetTimer(CGeneralSettings::AllTimer) != nullptr);
	//m_GS->TimerReset(CGeneralSettings::AllTimer);
	//message Loops
	while (msg.message != WM_QUIT)
	{
		//peeking Windows Message
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//OtherWise aonther actions
		else
		{
			//m_GS->TimerTick(CGeneralSettings::AllTimer);
			//timers tick

			if (!m_AppPaused)
			{
				//CalculateFrameStats();
				//m_GS->Update();
				//Update();
				//m_GS->Render();
				//Render();
				//m_GS->Late_Update();

			}
			else
			{
				Sleep(100);
			}
		}
	}
	return (int)msg.wParam;
}

bool CApiSetUP::Initialize()
{
	if (!InitMainWindow())
		return false;
	//if()
	OnResize();

	return true;
}

LRESULT CApiSetUP::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_AppPaused = true;
			//m_GS->TimerStop(CGeneralSettings::AllTimer);
		}
		else
		{
			m_AppPaused = false;
			//m_GS->TimerStart(CGeneralSettings::AllTimer);
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		g_ClientWidth = LOWORD(lParam);
		g_ClientHeight = HIWORD(lParam);
		/*m_ClientWidth = LOWORD(lParam);
		m_ClientHeight = HIWORD(lParam);*/
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing = true;
		//m_GS->TimerStop(CGeneralSettings::AllTimer);
		//mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing = false;
		//m_GS->TimerStart(CGeneralSettings::AllTimer);
		//m_Timer.Start();
		OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		else if ((int)wParam == VK_F2)
			//Set4xMsaaState(!m4xMsaaState);

			return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//
//void CApiSetUP::CalculateFrameStats()
//{
//	
//}

void CApiSetUP::OnResize()
{
	
}

bool CApiSetUP::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, g_ClientWidth, g_ClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	m_hMainWnd = CreateWindow(L"MainWnd", m_MainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, 0);
	if (!m_hMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}
	g_hInst = m_hAppInst;
	g_hWnd = m_hMainWnd;

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);
	//m_GS->GetDevice()->SetMainWnd(m_hMainWnd);
	return true;
}