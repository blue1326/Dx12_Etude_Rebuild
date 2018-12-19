#ifndef stdHeaders_h__
#define stdHeaders_h__

#include "windowsHeaders.h"
#include "STL.h"
#include "stream.h"
#include "d3dHeaders.h"
//it is now unknowned headers
#include "unknown.h"
#include "Macro.h"
#include "GlobalVariables.h"

#include "Functor.h"

#include "namespaces.h"



//necessary d3d12 libs
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

inline static float AspectRatio()
{
	return static_cast<float>(g_ClientWidth) / g_ClientHeight;
}

#endif // stdHeaders_h__
