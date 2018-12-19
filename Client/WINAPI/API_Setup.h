#ifndef API_Setup_h__
#define API_Setup_h__

#include "stdHeaders.h"

class CApiSetUP
{
public:
	CApiSetUP();
	CApiSetUP(HINSTANCE hInstance);
	CApiSetUP(const CApiSetUP& rhs) = delete;//��������� ����
	CApiSetUP& operator=(const CApiSetUP& rhs) = delete;//���Կ����� ����
	virtual ~CApiSetUP();
public:
	static CApiSetUP* GetApp();
	HINSTANCE	AppInst()const;
	HWND		MainWnd()const;

	virtual int Run();

	virtual bool Initialize();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	

protected:
	//virtual void CalculateFrameStats();
	virtual void OnResize();
	
protected:
	virtual bool InitMainWindow();

protected://����
	static CApiSetUP* m_App;
protected:
	HINSTANCE m_hAppInst = nullptr; // application instance handle
	HWND      m_hMainWnd = nullptr; // main window handle
	bool      m_AppPaused = false;  // is the application paused?
	bool      m_Minimized = false;  // is the application minimized?
	bool      m_Maximized = false;  // is the application maximized?
	bool      m_Resizing = false;   // are the resize bars being dragged?
	bool      m_FullscreenState = false;// fullscreen enabled

	std::wstring m_MainWndCaption = L"d3d App";

private://��� ������ �갡 ����
};



#endif // API_Setup_h__
