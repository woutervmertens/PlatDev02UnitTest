#pragma once

#pragma region
//**DX11
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

//DirectXMath Header Files:
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
using namespace DirectX;
//**EFFECTS11 (Helper for loading Effects (D3DX11))
#include "AdditionalLibraries/DX_Effects11/include/d3dx11effect.h"
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "AdditionalLibraries/DX_Effects11/lib/Debug/dxEffects11_d.lib")
#else 
#pragma comment(lib, "AdditionalLibraries/DX_Effects11/lib/Release/dxEffects11.lib")
#endif

//*DXERR (Helper for DX Error handling)
#include "AdditionalLibraries/DX_Error/include/dxerr.h"
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "AdditionalLibraries/DX_Error/lib/Debug/DxErr_d.lib")
#else 
#pragma comment(lib, "AdditionalLibraries/DX_Error/lib/Release/DxErr.lib")
#endif

//*DXTOOLKIT (Helper for loading DDS Textures (D3DX11))
#include "AdditionalLibraries/DX_Toolkit/include/DDSTextureLoader.h"
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "AdditionalLibraries/DX_Toolkit/lib/Debug/DirectXTK_d.lib")
#else 
#pragma comment(lib, "AdditionalLibraries/DX_Toolkit/lib/Release/DirectXTK.lib")
#endif

#pragma endregion DIRECTX INCLUDES



#pragma region
#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_
#endif

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers, disables fileopendialog
//PhysX:you must suppress its definition of the macros min and max, as these are common C++ method names in the SDK
#define NOMINMAX
// Windows Header Files:
#include <windows.h>
#include <Xinput.h>
//#pragma comment(lib, "XInput.lib")
//Changed for Windows 7 BUILD
#pragma comment(lib, "XINPUT9_1_0.LIB")

//#undef min;
//#undef max;
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <crtdbg.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <io.h>
#include <unordered_map>
#include <algorithm>	
#include <map>	
#include <vector>
using namespace std;
#pragma endregion WINDOWS INCLUDES

#pragma region
//*****************************************************************************
//Declare templates for releasing interfaces and deleting objects 
//*****************************************************************************
template<class Interface>
inline void SafeRelease(Interface &pInterfaceToRelease)
{
    if (pInterfaceToRelease != 0)
    {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = 0;
    }
}
template<class T>
inline void SafeDelete(T &pObjectToDelete)
{
	if (pObjectToDelete != 0)
    {
		delete(pObjectToDelete);
		pObjectToDelete = 0;
	}
}

template<typename T>
inline void Clamp(T& value, T hi, T lo)
{
	if(value > hi)
		value = hi;

	if(value < lo)
		value = lo;
}
#pragma endregion TEMPLATES & MACROS