#ifndef API_Derived_h__
#define API_Derived_h__

#include "stdHeaders.h"
#include "API_Setup.h"
#include "GeneralSettings.h"
class CApiSetUP_Derived : public CApiSetUP
{
public:
	explicit CApiSetUP_Derived(HINSTANCE hInstance);
	explicit CApiSetUP_Derived(const CApiSetUP_Derived& rhs) = delete;//��������� ����
	CApiSetUP_Derived& operator=(const CApiSetUP_Derived& rhs) = delete;//���Կ����� ����
	virtual ~CApiSetUP_Derived();
public:
	HINSTANCE	AppInst()const;
	HWND		MainWnd()const;

	int Run();

	virtual bool Initialize();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void SetGeneralSettings(std::unique_ptr<CGeneralSettings> gs);

private:
	
	virtual void CreateRtvAndDsvDescriptorHeaps();
	virtual void OnResize();
	

//private://����
	//static CApiSetUP* m_App;


private://��� ������ �갡 ����
	std::unique_ptr<CGeneralSettings> m_GS = nullptr;
};



#endif // API_Derived_h__
