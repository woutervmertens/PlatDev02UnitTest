#include "stdafx.h"
#include "Debug.h"
#include "ContentManager.h"
#include "VertexStructs.h"
#include "GameContext.h"

#include <fcntl.h>


HANDLE Debug::m_ConsoleHandle = 0;
ID3DX11Effect* Debug::m_pEffect = nullptr;
ID3DX11EffectTechnique* Debug::m_pTechnique = nullptr;
UINT Debug::m_BufferSize = 100;
ID3D11Buffer* Debug::m_pVertexBuffer = nullptr;
ID3D11InputLayout* Debug::m_pInputLayout = nullptr;
ID3DX11EffectMatrixVariable* Debug::m_pWvpVariable = nullptr;
vector<VertexPosCol> Debug::m_LineList = vector<VertexPosCol>();
vector<VertexPosCol> Debug::m_FixedLineList = vector<VertexPosCol>();
UINT Debug::m_FixedBufferSize = 0;
bool Debug::m_RendererEnabled = true;
wchar_t* Debug::m_ConvertBuffer = new wchar_t[m_ConvertBufferSize];


Debug::Debug(void)
{
}


Debug::~Debug(void)
{
}

void Debug::CleanUp()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);

	delete[] m_ConvertBuffer;
	m_ConvertBuffer = nullptr;
}

void Debug::InitLogger()
{

	if (AllocConsole())
	{

		// Redirect the CRT standard input, output, and error handles to the console
		FILE* pCout;
		freopen_s(&pCout, "CONIN$", "r", stdin);
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		freopen_s(&pCout, "CONOUT$", "w", stderr);

		//Clear the error state for each of the C++ standard stream objects. We need to do this, as
		//attempts to access the standard streams before they refer to a valid target will cause the
		//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
		//to always occur during startup regardless of whether anything has been read from or written to
		//the console or not.
		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();
		std::cin.clear();

		//Set ConsoleHandle
		m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		//Disable Close-Button
		HWND hwnd = GetConsoleWindow();
		if (hwnd != NULL)
		{
			HMENU hMenu = GetSystemMenu(hwnd, FALSE);
			if (hMenu != NULL) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}

}

void Debug::InitRenderer(ID3D11Device *pDevice, UINT bufferSize)
{
	m_BufferSize = bufferSize;
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Shaders/DebugRenderer.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	m_pWvpVariable = m_pEffect->GetVariableBySemantic("WORLDVIEWPROJECTION")->AsMatrix();
	if (!m_pWvpVariable->IsValid())
		LogWarning(L"Debug Renderer: Invalid Shader Variable! (WVP)");

	//Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	pDevice->CreateInputLayout(layout, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout);

	CreateFixedLineList();
	CreateVertexBuffer(pDevice);
}

void Debug::CreateVertexBuffer(ID3D11Device *pDevice)
{
	SafeRelease(m_pVertexBuffer);

	//Vertexbuffer
	D3D11_BUFFER_DESC buffDesc;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = sizeof(VertexPosCol) * (m_BufferSize + m_FixedBufferSize);
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags = 0;

	//Crashes on some systems :s
	//D3D11_SUBRESOURCE_DATA initData = {0};
	//initData.pSysMem = m_FixedLineList.data();
	//pDevice->CreateBuffer(&buffDesc, &initData, &m_pVertexBuffer);

	//Solution: Create buffer without initial data
	pDevice->CreateBuffer(&buffDesc, nullptr, &m_pVertexBuffer);

	if (m_FixedBufferSize > 0)
	{
		//Map Fixed data manually
		ID3D11DeviceContext* pDeviceContext;
		pDevice->GetImmediateContext(&pDeviceContext);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
		memcpy(mappedResource.pData, &m_FixedLineList[0], sizeof(VertexPosCol) * m_FixedBufferSize);
		pDeviceContext->Unmap(m_pVertexBuffer, 0);
		pDeviceContext->Release();
	}
}

void Debug::CreateFixedLineList()
{
	//*GRID*
	UINT numGridLines = 20;
	float gridSpacing = 1.0f;

	float startOffset = -((int)numGridLines / 2)*gridSpacing;
	float size = (numGridLines - 1) * gridSpacing;
	auto gridColor = (XMFLOAT4)Colors::LightGray;
	for (UINT i = 0; i < numGridLines; ++i)
	{
		//VERTICAL
		float lineOffset = startOffset + gridSpacing * i;
		auto vertStart = XMFLOAT3(startOffset, 0, lineOffset);
		m_FixedLineList.push_back(VertexPosCol(vertStart, gridColor));
		vertStart.x += size;
		m_FixedLineList.push_back(VertexPosCol(vertStart, gridColor));

		//HORIZONTAL
		vertStart = XMFLOAT3(lineOffset, 0, startOffset);
		m_FixedLineList.push_back(VertexPosCol(vertStart, gridColor));
		vertStart.z += size;
		m_FixedLineList.push_back(VertexPosCol(vertStart, gridColor));
	}

	//*AXIS
	m_FixedLineList.push_back(VertexPosCol(XMFLOAT3(0, 0, 0), (XMFLOAT4)Colors::DarkRed));
	m_FixedLineList.push_back(VertexPosCol(XMFLOAT3(30, 0, 0), (XMFLOAT4)Colors::DarkRed));
	m_FixedLineList.push_back(VertexPosCol(XMFLOAT3(0, 0, 0), (XMFLOAT4)Colors::DarkGreen));
	m_FixedLineList.push_back(VertexPosCol(XMFLOAT3(0, 30, 0), (XMFLOAT4)Colors::DarkGreen));
	m_FixedLineList.push_back(VertexPosCol(XMFLOAT3(0, 0, 0), (XMFLOAT4)Colors::DarkBlue));
	m_FixedLineList.push_back(VertexPosCol(XMFLOAT3(0, 0, 30), (XMFLOAT4)Colors::DarkBlue));

	//@END!
	m_FixedBufferSize = m_FixedLineList.size();
}

#pragma region
void Debug::LogFormat(LogLevel level, const wchar_t* format, ...)
{
	va_list ap;

	va_start(ap, format);
	_vsnwprintf_s(&m_ConvertBuffer[0], m_ConvertBufferSize, m_ConvertBufferSize, format, ap);
	va_end(ap);
	Log(level, wstring(&m_ConvertBuffer[0]));
}

bool Debug::LogHResult(HRESULT hr, const wstring& originInfo, const wstring& msgOnSuccess)
{
	if (!LogHResult(hr, originInfo))
	{
		Log(LogLevel::Info, msgOnSuccess);
		return false;
	}

	return true;
}

bool Debug::LogHResult(HRESULT hr, const wstring& originInfo)
{
	if (SUCCEEDED(hr))
		return false;

	//Extract error message from HRESULT
	if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
		hr = HRESULT_CODE(hr);

	wstringstream ss;
	if (originInfo.size() != 0)
	{
		ss << ">ORIGIN:\n";
		ss << originInfo;
		ss << "\n\n";
	}
	ss << ">ERROR:\n";

	TCHAR* errorMsg;
	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorMsg, 0, NULL) != 0)
	{
		ss << errorMsg;
	}
	else
	{
		ss << "Could not find a description for error ";
		ss << hr;
		ss << ".";
	}

	Log(LogLevel::HResult, ss.str());

	return true;
}

void Debug::Log(LogLevel level, const wstring& msg)
{
	if (!m_ConsoleHandle)
		return;

	switch (level)
	{
	case LogLevel::Info:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		wcout << L"[INFO]    >>";
		//SetConsoleTextAttribute(m_ConsoleHandle);
		break;
	case LogLevel::Warning:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		wcout << L"[WARNING] >>";

		break;
	case LogLevel::HResult:
	case LogLevel::Error:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		wcout << L"[ERROR]   >>";

		MessageBox(nullptr, msg.c_str(), L"ERROR", MB_OK | MB_ICONERROR);
		break;
	}

	wcout << msg << endl;
}
#pragma endregion Logger Methods
#pragma region
void Debug::ToggleDebugRenderer()
{
	m_RendererEnabled = !m_RendererEnabled;
}

void Debug::DrawLine(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT4 color)
{
	if (!m_RendererEnabled)
		return;

	m_LineList.push_back(VertexPosCol(start, color));
	m_LineList.push_back(VertexPosCol(end, color));
}

void Debug::DrawLine(XMFLOAT3 start, XMFLOAT4 colorStart, XMFLOAT3 end, XMFLOAT4 colorEnd)
{
	if (!m_RendererEnabled)
		return;

	m_LineList.push_back(VertexPosCol(start, colorStart));
	m_LineList.push_back(VertexPosCol(end, colorEnd));
}

void Debug::Draw(GameContext* pContext)
{
	if (!m_RendererEnabled)
		return;

	UINT size = m_LineList.size();

	if ((size + m_FixedBufferSize) <= 0)
		return;
	else if (size > m_BufferSize)
	{
		LogWarning(L"Debug Renderer: Increasing Vertexbuffer Size!");
		m_BufferSize = size;
		CreateVertexBuffer(pContext->GetDevice());
	}

	auto pDevContext = pContext->GetDeviceContext();

	if (size>0)
	{
		//Map Vertex Data
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		pDevContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
		memcpy(&(((VertexPosCol*)mappedResource.pData)[m_FixedBufferSize]), &m_LineList[0], sizeof(VertexPosCol) * size);
		pDevContext->Unmap(m_pVertexBuffer, 0);
	}

	//Set Render Pipeline
	pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	UINT stride = sizeof(VertexPosCol);
	UINT offset = 0;
	pDevContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pDevContext->IASetInputLayout(m_pInputLayout);

	auto viewProj = pContext->GetCamera()->GetViewProjection();
	XMMATRIX wvp = XMMatrixIdentity() * XMLoadFloat4x4(&viewProj);
	XMFLOAT4X4 wvpConverted;
	XMStoreFloat4x4(&wvpConverted, wvp);

	m_pWvpVariable->SetMatrix(reinterpret_cast<float*>(&wvpConverted));

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		m_pTechnique->GetPassByIndex(i)->Apply(0, pDevContext);
		pDevContext->Draw(size + m_FixedBufferSize, 0);
	}

	m_LineList.clear();
}
#pragma endregion Renderer Methods