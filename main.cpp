#include "stdafx.h"


//ReportLiveObjects:
//To use any of these GUID values, you must include Initguid.h before you include DXGIDebug.h in your code.
#include <Initguid.h>
#include <dxgidebug.h>

#include "MainGame.h"

//int wmain(int argc, wchar_t* argv[])
//{
//	wWinMain(GetModuleHandle(0),0,0,SW_SHOW);
//}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   wchar_t* cmdLine, int showCmd)
{
	//notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL,0);

	// Enable run-time memory leak check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

		typedef HRESULT(__stdcall *fPtr)(const IID&, void**); 
		HMODULE hDll = LoadLibrary(L"dxgidebug.dll"); 
		fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface"); 

		IDXGIDebug* pDXGIDebug;
		DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDXGIDebug);
		//_CrtSetBreakAlloc(414);
	#endif

	auto *gamePtr = new MainGame();
	gamePtr->RunGame(hInstance);
	delete gamePtr;

	#if defined(DEBUG) | defined(_DEBUG)
	// unresolved external  
	if(pDXGIDebug) pDXGIDebug->ReportLiveObjects(DXGI_DEBUG_ALL,DXGI_DEBUG_RLO_ALL); 
    pDXGIDebug->Release();
	#endif
}
