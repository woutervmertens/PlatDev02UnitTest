#pragma once

class GameContext;
struct VertexPosCol;

class Debug
{
public:

	static void InitLogger();
	static void InitRenderer(ID3D11Device *pDevice, UINT bufferSize = 100);
	static void ToggleDebugRenderer();
	static void CleanUp();

	//LOGGING
	enum LogLevel
	{
		Info,
		Warning,
		Error,
		HResult
	};

	static void LogInfo(const wstring& msg) { Log(LogLevel::Info, msg); }
	static void LogWarning(const wstring& msg) { Log(LogLevel::Warning, msg); }
	static void LogError(const wstring& msg) { Log(LogLevel::Error, msg); }
	static bool LogHResult(HRESULT hr, const wstring& originInfo);
	static bool LogHResult(HRESULT hr, const wstring& originInfo, const wstring& msgOnSuccess);
	static void Log(LogLevel level, const wstring& msg);
	static void LogFormat(LogLevel level, const wchar_t* format, ...);

	//RENDERING
	static void Draw(GameContext* pContext);
	static void DrawLine(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT4 color = (XMFLOAT4)Colors::Red);
	static void DrawLine(XMFLOAT3 start, XMFLOAT4 colorStart, XMFLOAT3 end, XMFLOAT4 colorEnd);


private:
	Debug(void);
	~Debug(void);

	//LOGGING
	static HANDLE m_ConsoleHandle;
	static wchar_t* m_ConvertBuffer;
	static const size_t m_ConvertBufferSize = 1024;

	//RENDERING
	static void CreateVertexBuffer(ID3D11Device *pDevice);
	static void CreateFixedLineList();
	static ID3DX11Effect* m_pEffect;
	static ID3DX11EffectTechnique* m_pTechnique;
	static UINT m_BufferSize, m_FixedBufferSize;
	static ID3D11InputLayout *m_pInputLayout;
	static ID3D11Buffer *m_pVertexBuffer;
	static ID3DX11EffectMatrixVariable *m_pWvpVariable;
	static vector<VertexPosCol> m_LineList;
	static vector<VertexPosCol> m_FixedLineList;
	static bool m_RendererEnabled;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Debug(const Debug& t);
	Debug& operator=(const Debug& t);
};

