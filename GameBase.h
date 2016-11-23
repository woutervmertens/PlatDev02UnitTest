#pragma once


struct GameSettings
{
	GameSettings():
		BackbufferWidth(1280),
		BackbufferHeight(720),
		Title(L"GP1_Framework"),
		VBLWait(true),
		ClearColor(Colors::CornflowerBlue)
	{}

	int BackbufferWidth;
	int BackbufferHeight;
	wstring Title;
	bool VBLWait;
	const float* ClearColor;
};


class GameBase //final
{
public:
	GameBase(void);
	virtual ~GameBase(void);
	HRESULT RunGame(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowsProcedureStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:

	virtual void Initialize() = 0;
	virtual void OnPreparingGame(GameSettings& settings) = 0;
	virtual LRESULT WindowsProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { return -1; }

	//Direct3D variables
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D *m_pTexDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;
	IDXGISwapChain *m_pSwapChain;
	D3D11_VIEWPORT m_Viewport;
	bool m_IsActive;

private:
	//Functions
	HRESULT CreateWindowClass();
	HRESULT OpenWindow();
	HRESULT InitializeDirect3D();

	void CleanupDirect3D();
	void StateChanged(int state, bool active);

	//Handle Windows Messages
	LRESULT WindowsProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//Window variables
	HINSTANCE m_hInstance;
	HWND m_WindowHandle;
	wstring m_WindowClassName;
	WNDCLASS m_WindowClass;

	GameSettings m_GameSettings;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameBase(const GameBase& t);
	GameBase& operator=(const GameBase& t);
};

