
#include "stdHeaders.h"
//#include "API_Setup.h"
#include "API_Derived.h"
#include "Timer.h"
#include "dxException.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		CApiSetUP_Derived theApp(hInstance);
		theApp.SetGeneralSettings(std::unique_ptr<CGeneralSettings>(
			new CGeneralSettings(
				std::shared_ptr<DxDevice>(new DxDevice),
				std::shared_ptr<CTimer>(new CTimer),
				std::shared_ptr<CTimer>(new CTimer))
			));
		
		if (!theApp.Initialize())
			return 0;

		return theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}