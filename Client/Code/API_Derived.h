#ifndef API_Derived_h__
#define API_Derived_h__

#include "stdHeaders.h"
#include "API_Setup.h"
#include "GeneralSettings.h"
class CApiSetUP_Derived : public CApiSetUP
{
public:
	explicit CApiSetUP_Derived(HINSTANCE hInstance);
	explicit CApiSetUP_Derived(const CApiSetUP_Derived& rhs) = delete;//복사생성자 삭제
	CApiSetUP_Derived& operator=(const CApiSetUP_Derived& rhs) = delete;//대입연산자 삭제
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
	

//private://변수
	//static CApiSetUP* m_App;


private://모든 권한은 얘가 가짐
	std::unique_ptr<CGeneralSettings> m_GS = nullptr;
};



#endif // API_Derived_h__
