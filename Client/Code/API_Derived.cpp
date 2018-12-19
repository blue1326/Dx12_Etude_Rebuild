#include "API_Derived.h"
#include <windowsx.h>


HWND g_hWnd;
HINSTANCE g_hInst;
int g_ClientWidth = 800;
int g_ClientHeight = 600;
bool g_DeviceInitState = false;
XMMATRIX g_matView = XMMatrixIdentity();
XMMATRIX g_matProj = XMMatrixIdentity();

//CApiSetUP* CApiSetUP_Derived::m_App = nullptr; //static initalize

//LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
//	// before CreateWindow returns, and thus before mhMainWnd is valid.
//	return CApiSetUP_Derived::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
//}




CApiSetUP_Derived::CApiSetUP_Derived(HINSTANCE hInstance)
{
	m_hAppInst = hInstance;
	assert(m_App == nullptr); //running on debug state
	m_App = this;
}

CApiSetUP_Derived::~CApiSetUP_Derived()
{
	//not yet using...
}


HINSTANCE CApiSetUP_Derived::AppInst() const
{
	return m_hAppInst;
}

HWND CApiSetUP_Derived::MainWnd() const
{
	return m_hMainWnd;
}

int CApiSetUP_Derived::Run()
{
	MSG msg = { 0 };
	assert(m_GS != nullptr);
	//need timer
	assert(m_GS->GetTimer(CGeneralSettings::AllTimer) != nullptr);
	m_GS->TimerReset(CGeneralSettings::AllTimer);
	m_GS->SetFramelateLimit(60.f);
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
			m_GS->TimerTick(CGeneralSettings::AllTimer);
			//timers tick

			if (!m_AppPaused)
			{
				if (m_GS->CalculateFrameStats(g_hWnd, m_MainWndCaption))
				{
					m_GS->Update();
					//Update();
					m_GS->Render();
					//Render();
					m_GS->Last_Update();
				}

			}
			else
			{
				Sleep(100);
			}
		}
	}
	return (int)msg.wParam;
}

bool CApiSetUP_Derived::Initialize()
{
	m_MainWndCaption = L"Dx12 Etude";
	if (!InitMainWindow())
		return false;
	if (!m_GS->InitGeneralSetting())
		return false;
	OnResize();

	return true;
}

LRESULT CApiSetUP_Derived::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			m_GS->TimerStop(CGeneralSettings::AllTimer);
		}
		else
		{
			m_AppPaused = false;
			m_GS->TimerStart(CGeneralSettings::AllTimer);
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		g_ClientWidth = LOWORD(lParam);
		g_ClientHeight = HIWORD(lParam);
		/*m_ClientWidth = LOWORD(lParam);
		m_ClientHeight = HIWORD(lParam);*/
		if (g_DeviceInitState ==true)
			//m_GS->GetDevice()->DeviceInitState() == true)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_Maximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (m_Minimized)
				{
					m_AppPaused = false;
					m_Minimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (m_Maximized)
				{
					m_AppPaused = false;
					m_Maximized = false;
					OnResize();
				}
				else if (m_Resizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing = true;
		m_GS->TimerStop(CGeneralSettings::AllTimer);
		//mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing = false;
		m_GS->TimerStart(CGeneralSettings::AllTimer);
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

void CApiSetUP_Derived::SetGeneralSettings(std::unique_ptr<CGeneralSettings> gs)
{
	m_GS = std::move(gs);
}

//void CApiSetUP_Derived::CalculateFrameStats()
//{
//	m_GS->CalculateFrameStats(g_hWnd, m_MainWndCaption);
//}

void CApiSetUP_Derived::CreateRtvAndDsvDescriptorHeaps()
{

}

void CApiSetUP_Derived::OnResize()
{
	m_GS->OnResize();
}

