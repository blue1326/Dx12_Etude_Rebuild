#ifndef GeneralSettings_h__
#define GeneralSettings_h__
#include "stdHeaders.h"
#include "DxDevice.h"
#include "Component.h"

#include "Scene.h"
class CGeneralSettings 
{
public:
	enum TimerDef { MainTimer, SubTimer,AllTimer };
public:
	CGeneralSettings(const std::shared_ptr<DxDevice> Device ,const std::shared_ptr<CTimer> Timer1, const std::shared_ptr<CTimer> Timer2);
	~CGeneralSettings();
public:
	bool InitGeneralSetting();
	HRESULT InitScene();
	void Update();
	void Last_Update();
	void Render();
	//void SetClientSize(int width, int height);//юс╫ц
	void OnResize();
public: //for Timer method
	std::shared_ptr<CTimer> GetTimer(TimerDef td);
	void TimerReset(TimerDef td);
	void TimerStart(TimerDef td);
	void TimerStop(TimerDef td);
	void TimerTick(TimerDef td);

public: //for framelate

public: //for frame Count
	bool CalculateFrameStats(const HWND& mainWnd, std::wstring caption);
private: //for frame Limit
	float m_CallPerSec;
	float m_fTimeAcc;
public: //for frame Limit
	void SetFramelateLimit(const float& _Limit);
	
public: //for DxDevice method
	std::shared_ptr<DxDevice> GetDevice();
	HRESULT InitDxDevice();
private://for components
	HRESULT InitComponents();
private:
	const std::shared_ptr<CTimer> m_Timer1 = nullptr;
	
	const std::shared_ptr<CTimer> m_Timer2 = nullptr;
	const std::shared_ptr<DxDevice> m_DxDevice = nullptr;
	
	

	std::shared_ptr<CScene> m_Scene = nullptr;

};

#endif // GeneralSettings_h__
