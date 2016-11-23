#include "stdafx.h"
#include "CylinderPosColorNorm.h"
#include "VertexStructs.h"
#include "ContentManager.h"
#include "Debug.h"


CylinderPosColorNorm::CylinderPosColorNorm(float radius, UINT radiusSteps, float height, XMFLOAT4 color):
		m_Radius(radius),
		m_RadiusSteps(radiusSteps),
		m_Height(height),
		m_Color(color),
		m_pVertexLayout(nullptr),
		m_pVertexBuffer(nullptr),
		m_pIndexBuffer(nullptr),
		m_pEffect(nullptr),
		m_pTechnique(nullptr),
		m_pMatWorldViewProjVariable(nullptr),
		m_pMatWorldVariable(nullptr),
		m_NumVerts(0), 
		m_NumIndices(0)
{
}


CylinderPosColorNorm::~CylinderPosColorNorm(void)
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pVertexLayout);
}

void CylinderPosColorNorm::Initialize(GameContext* pContext)
{
	//Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Shaders/PosColNorm3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if(!m_pMatWorldViewProjVariable->IsValid())Debug::LogWarning(L"CylinderPosColorNorm::Initialize() - WvpVariable is invalid!");
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	if(!m_pMatWorldVariable->IsValid())Debug::LogWarning(L"CylinderPosColorNorm::Initialize() - WorldVariable is invalid!");

	//InputLayout
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof( vertexDesc ) / sizeof( vertexDesc[0] );

  	// Create the input layout
    D3DX11_PASS_DESC passDesc;
    m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	auto hr = pContext->GetDevice()->CreateInputLayout(
		vertexDesc, 
		numElements, 
		passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, 
		&m_pVertexLayout);
	Debug::LogHResult(hr, L"Failed to Create InputLayout");

	//**CYLINDER**
	

	//Vertices
	vector<VertexPosColNorm> vertices;
	float interval = XM_2PI/m_RadiusSteps;
	for(UINT step = 0; step < m_RadiusSteps; ++step)
	{
		float circleRadius = step * interval;
		XMFLOAT3 pos = XMFLOAT3(m_Radius * cos(circleRadius), m_Height/2.0f, m_Radius * sin(circleRadius));
		XMVECTOR vecNorm = XMVectorSet(pos.x, 0, pos.z, 0);
		vecNorm = XMVector3Normalize(vecNorm);
		XMFLOAT3 norm;
		XMStoreFloat3(&norm, vecNorm);

		vertices.push_back(VertexPosColNorm(pos , m_Color, XMFLOAT3(0,1,0)));

		vertices.push_back(VertexPosColNorm(pos , m_Color, norm));
		pos.y *= -1;
		vertices.push_back(VertexPosColNorm(pos , m_Color, norm));
		vertices.push_back(VertexPosColNorm(pos , m_Color, XMFLOAT3(0,-1,0)));
	}

	vertices.push_back(VertexPosColNorm(XMFLOAT3(0,m_Height/2.0f,0) , m_Color, XMFLOAT3(0,1,0)));
	vertices.push_back(VertexPosColNorm(XMFLOAT3(0,-m_Height/2.0f,0) , m_Color, XMFLOAT3(0,-1,0)));

	m_NumVerts = vertices.size();

	vector<DWORD> indices;
	for(UINT i = 0; i < m_NumVerts-2; i+=4)
	{
		indices.push_back(m_NumVerts-2);
		indices.push_back(i);
		indices.push_back((i+4)%(m_NumVerts-2));
		indices.push_back(i+1);
		indices.push_back(i+2);
		indices.push_back((i+6)%(m_NumVerts-2));
		indices.push_back((i+6)%(m_NumVerts-2));
		indices.push_back((i+5)%(m_NumVerts-2));
		indices.push_back(i+1);
		indices.push_back(i+3);
		indices.push_back(m_NumVerts - 1);
		indices.push_back((i+7)%(m_NumVerts-2));
	}

	m_NumIndices = indices.size();

	//Vertexbuffer
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = {0};

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof( VertexPosColNorm ) * m_NumVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = vertices.data();
	hr = pContext->GetDevice()->CreateBuffer( &bd, &initData, &m_pVertexBuffer );
	Debug::LogHResult(hr, L"Failed to Create Vertexbuffer");

	//Indexbuffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth =  sizeof(DWORD) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();
	hr = pContext->GetDevice()->CreateBuffer( &bd, &initData, &m_pIndexBuffer );
	Debug::LogHResult(hr, L"Failed to Create Indexbuffer");
}

void CylinderPosColorNorm::Update(GameContext* pContext)
{

}

void CylinderPosColorNorm::Draw(GameContext* pContext)
{
	XMMATRIX world = XMLoadFloat4x4(&m_WorldMatrix);
	XMMATRIX viewProj = XMLoadFloat4x4(&pContext->GetCamera()->GetViewProjection());
	XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));

	// Set vertex buffer
	UINT stride = sizeof( VertexPosColNorm );
    UINT offset = 0;
	auto deviceContext = pContext->GetDeviceContext();
	deviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	// Set index buffer
	deviceContext->IASetIndexBuffer(m_pIndexBuffer,DXGI_FORMAT_R32_UINT,0);

	// Set the input layout
	deviceContext->IASetInputLayout( m_pVertexLayout );

	// Set primitive topology
    deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Render a torus
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for(UINT p=0; p< techDesc.Passes;++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, deviceContext);
		deviceContext->DrawIndexed( m_NumIndices, 0, 0);
	}
}
