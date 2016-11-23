#include "stdafx.h"
#include "GameBase.h"
#include "SceneManager.h"
#include "ContentManager.h"
#include "Debug.h"


GameBase::GameBase(void)
	:m_hInstance(0)
	,m_WindowHandle(0)
	,m_WindowClassName(L"GP1Framework")
	,m_WindowClass()
	,m_pDevice(nullptr)
	,m_pDeviceContext(nullptr)
	,m_pSwapChain(nullptr)
	,m_pRenderTargetView(nullptr)
	,m_pTexDepthStencilBuffer(nullptr)
	,m_pDepthStencilView(nullptr)
	,m_GameSettings(GameSettings())

{
	if(!XMVerifyCPUSupport())	MessageBoxW(0, L"XMVerifyCPUSupport: failed", L"ERROR", MB_OK|MB_ICONERROR);
}

GameBase::~GameBase(void)
{
	ContentManager::Unload();
	SceneManager::Destroy();
	Debug::CleanUp();
	CleanupDirect3D();
}

void GameBase::CleanupDirect3D()
{
	SafeRelease(m_pRenderTargetView);
	SafeRelease(m_pDepthStencilView);
	SafeRelease(m_pSwapChain);
	SafeRelease(m_pTexDepthStencilBuffer);

	if(m_pDeviceContext)
	{
		m_pDeviceContext->ClearState();
		m_pDeviceContext->Flush();
		SafeRelease(m_pDeviceContext);
	}

	SafeRelease(m_pDevice);

}

void GameBase::StateChanged(int state, bool active)
{
	switch (state)
	{
		//WINDOW ACTIVE/INACTIVE
	case 0:
		if (m_IsActive != active)
		{
			m_IsActive = active;
			SceneManager::WindowStateChanged(state, active);
		}
		break;
		//INPUT ACTIVE/INACTIVE
	case 1:
		SceneManager::m_InputEnabled = active;
		break;
	}
}

HRESULT GameBase::RunGame(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	HRESULT hr;

	//Initialize Debug Logger (Needed for logging during Initialization)
	Debug::InitLogger();

	//PREPARE GAME
	OnPreparingGame(m_GameSettings);

	//1. Create the Window Class
	hr = CreateWindowClass();
	if(Debug::LogHResult(hr, L"GameBase::RunGame", L"Window Class Created!"))
		return hr;

	//2. Open the window 
	hr = OpenWindow();
	if(Debug::LogHResult(hr, L"GameBase::RunGame", L"Window Opened"))
		return hr;

	//3. Direct3D Init
	hr = InitializeDirect3D();
	if(Debug::LogHResult(hr, L"GameBase::RunGame", L"Direct3D Initialized"))
		return hr;

	//4. Initialize Managers
	ContentManager::Initialize(m_pDevice);
	Debug::InitRenderer(m_pDevice); //Only possible after DeviceCreation & ContentManager Init

	Debug::LogInfo(L"Managers Initialized!");

	//5. Initialize the GameBase
	Initialize();
	Debug::LogInfo(L"GameBase Initialized!");
	SceneManager::Initialize(m_pDevice, m_pDeviceContext);
	Debug::LogInfo(L"Scenes Initialized!");

	//5. Start the message pump
	MSG msg = {0};
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//CLEAR TARGET
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView,m_GameSettings.ClearColor);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//GAME LOOP
		SceneManager::Update();
		SceneManager::Draw();

		//PRESENT
		m_pSwapChain->Present(m_GameSettings.VBLWait?1:0,0);
	}
	
	//6. Finished
	Debug::LogInfo(L"Finished");

	return (int)msg.wParam;
}

HRESULT GameBase::CreateWindowClass()
{
	ZeroMemory( &m_WindowClass, sizeof( WNDCLASS ) );
	m_WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WindowClass.hIcon = nullptr;
	m_WindowClass.hbrBackground = nullptr;
	m_WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	m_WindowClass.lpfnWndProc = WindowsProcedureStatic;
	m_WindowClass.hInstance = GetModuleHandle(0);
	m_WindowClass.lpszClassName = m_WindowClassName.c_str();

	if(!RegisterClass( &m_WindowClass))
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}
	return S_OK;
}

HRESULT GameBase::OpenWindow()
{
	//Calculate position (centered)
	RECT R = {0,0,m_GameSettings.BackbufferWidth,m_GameSettings.BackbufferHeight};
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	LONG windowWidth = R.right - R.left;
	LONG windowHeight = R.bottom - R.top;

	//Calculate the position of the window to be centered on the desktop
	int posX = GetSystemMetrics(SM_CXSCREEN)/2-windowWidth/2;		//calculate the left position of the window
	int posY = GetSystemMetrics(SM_CYSCREEN)/2-windowHeight/2;	//calculate the top position of the window

	
	m_WindowHandle = CreateWindowW( m_WindowClassName.c_str(),
		m_GameSettings.Title.c_str(), 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 
		posX, 
		posY, 
		windowWidth, 
		windowHeight, 
		NULL, 
		0, 
		m_hInstance, 
		this);//'this' is retrieved in WindowsProcedureStatic by lpCreateParams 

	if(m_WindowHandle == NULL)
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}

	//Show the window
	ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
	return S_OK;
}

HRESULT GameBase::InitializeDirect3D()
{
	HRESULT hr;

	//Create DXGIFactory
	IDXGIFactory* pDXGIFactory;
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&pDXGIFactory));
	if(Debug::LogHResult(hr, L"InitializeDirect3D: ::CreateDXGIFactory(...)"))return hr;

	//Allows DXGI to monitor an application's message queue for the alt-enter key sequence 
	//(which causes the application to switch from windowed to full screen or vice versa).
	//hr = pDXGIFactory->MakeWindowAssociation(m_WindowHandle, 0);
	//if(AlertOnFail(hr, L"InitializeDirect3D: ::MakeWindowAssociation(...)"))return hr;

	//Get all Graphics Adapters
	IDXGIAdapter* pDXGIAdapter = nullptr;
	UINT i = 0;
	wstringstream ss;
	while(pDXGIFactory->EnumAdapters(i, &pDXGIAdapter) != DXGI_ERROR_NOT_FOUND) 
	{ 
		DXGI_ADAPTER_DESC adapterDescriptor;
		pDXGIAdapter->GetDesc(&adapterDescriptor);
		ss << L"Adapter "<< i << L": " << adapterDescriptor.Description <<  endl;
		pDXGIAdapter->Release();
		++i; 
	} 
	ss << i << L" Adapter(s) found" << endl;
	Debug::LogInfo(ss.str());

	// Could use the first adapter
	hr = pDXGIFactory->EnumAdapters(0, &pDXGIAdapter);

	//4.2.1 Create the Device and Context
	UINT createDeviceFlags = 0;

	#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0  ;
	hr = D3D11CreateDevice(		pDXGIAdapter,//when pDXGIAdapter is used, nex param must be D3D_DRIVER_TYPE_UNKNOWN
								D3D_DRIVER_TYPE_UNKNOWN,
								NULL,
								createDeviceFlags,
								0,0,
								D3D11_SDK_VERSION,
								&m_pDevice,
								&featureLevel,
								&m_pDeviceContext
	);
	pDXGIAdapter->Release();
	if(Debug::LogHResult(hr, L"InitializeDirect3D: D3D11CreateDevice(...)"))return hr;
	if(featureLevel!=D3D_FEATURE_LEVEL_11_0)
		if(Debug::LogHResult(S_FALSE, L"InitializeDirect3D: Feature level 11.0 unsupported"))return hr;

	//4.2.3 Describe the swapchain
	//IDXGIDevice* pDXGIDevice = nullptr;
	//hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
	//if(AlertOnFail(hr, L"InitializeDirect3D: m_pDevice->QueryInterface pDXGIDevice"))return hr;

	//Create Swapchain descriptor
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc.Width = m_GameSettings.BackbufferWidth;
	swapChainDesc.BufferDesc.Height = m_GameSettings.BackbufferHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_WindowHandle;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hr = pDXGIFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);

	//release temporary resources
	pDXGIFactory->Release();

	//4.2.5 Create the Render Target View
	ID3D11Texture2D *pRenderTargetBuffer(nullptr);

	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pRenderTargetBuffer));
	if(Debug::LogHResult(hr, L"InitializeDirect3D: m_pSwapChain->GetBuffer"))return hr;

	hr = m_pDevice->CreateRenderTargetView(pRenderTargetBuffer, 0, &m_pRenderTargetView);
	if(Debug::LogHResult(hr, L"InitializeDirect3D: m_pDevice->CreateRenderTargetView"))return hr;

	if(pRenderTargetBuffer)pRenderTargetBuffer->Release();

	//4.2.6 Create the Depth/Stencil Buffer and View
	D3D11_TEXTURE2D_DESC depthStencilDesc={0};
	depthStencilDesc.Width     = m_GameSettings.BackbufferWidth;
	depthStencilDesc.Height    = m_GameSettings.BackbufferHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	hr = m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pTexDepthStencilBuffer);
	if(Debug::LogHResult(hr, L"InitializeDirect3D: m_pDevice->CreateTexture2D  m_pTexDepthStencilBuffer"))return hr;
	
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc={};
    depthStencilViewDesc.Format = depthStencilDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_pDevice->CreateDepthStencilView(m_pTexDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if(Debug::LogHResult(hr, L"InitializeDirect3D: m_pDevice->CreateDepthStencilView "))return hr;
		
	//4.2.7 Bind the Views to the Output Merger Stage
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	
	//4.2.8 Set the Viewport
	m_Viewport.Width	= (FLOAT)m_GameSettings.BackbufferWidth;
	m_Viewport.Height	= (FLOAT)m_GameSettings.BackbufferHeight;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_pDeviceContext->RSSetViewports(1,&m_Viewport);

	return S_OK;
}

#pragma region
LRESULT CALLBACK GameBase::WindowsProcedureStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_CREATE)
	{
		CREATESTRUCT *pCS = (CREATESTRUCT*)lParam;
		SetWindowLongW(hWnd, GWLP_USERDATA, (LONG)pCS->lpCreateParams);
	}
	else
	{
		GameBase* pThisGame = (GameBase*)GetWindowLongW(hWnd, GWLP_USERDATA);
		if(pThisGame) return pThisGame->WindowsProcedure(hWnd, message, wParam, lParam);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT GameBase::WindowsProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_ACTIVATE:
	{
		if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)StateChanged(1, true);
		else StateChanged(1, false);

		return 0;
	}
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) StateChanged(0, false);
		else if (wParam == SIZE_RESTORED) StateChanged(0, true);
		return 0;
	case WM_SETFOCUS:
		if ((HWND)wParam == m_WindowHandle)
		{
			StateChanged(1, true);
			return 0;
		}
		break;
	case WM_KILLFOCUS:
		if ((HWND)wParam == m_WindowHandle)
		{
			StateChanged(1, false);
			return 0;
		}
		break;
	case WM_ENTERSIZEMOVE:
		StateChanged(0, false);
		StateChanged(1, false);
		break;
	case WM_EXITSIZEMOVE:
		StateChanged(0, true);
		StateChanged(1, true);
		break;
	}

	if (m_IsActive && WindowsProcedureHook(hWnd, message, wParam, lParam) == 0)
		return 0;

	return DefWindowProc(hWnd, message, wParam, lParam);
}


#pragma endregion Windows Procedures