#include "stdafx.h"
#include "CubePosColorNorm.h"
#include "VertexStructs.h"
#include "ContentManager.h"


CubePosColorNorm::CubePosColorNorm(float width, float height, float depth):
	m_Width(width),
	m_Height(height),
	m_Depth(depth),
	m_pVertexLayout(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pEffect(nullptr),
	m_pTechnique(nullptr),
	m_pMatWorldViewProjVariable(nullptr),
	m_pMatWorldVariable(nullptr)
{
}


CubePosColorNorm::~CubePosColorNorm(void)
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pVertexLayout);
}

void CubePosColorNorm::Initialize(GameContext* pContext)
{
	//Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Shaders/PosColNorm3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();

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

	//Vertex Buffer
	VertexPosColNorm vertices[NUMVERTICES] = {};
	int index=0;
	//front red
	vertices[index].Position		= XMFLOAT3(-m_Width/2, -m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 0, -1);
	vertices[index++].Color	= XMFLOAT4(1,0,0,1);
	vertices[index].Position		= XMFLOAT3(-m_Width/2,  m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 0, -1);
	vertices[index++].Color	= XMFLOAT4(1,0,0,1);
	vertices[index].Position		= XMFLOAT3( m_Width/2, -m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 0, -1);
	vertices[index++].Color	= XMFLOAT4(1,0,0,1);
	vertices[index].Position		= XMFLOAT3( m_Width/2,  m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 0, -1);
	vertices[index++].Color	= XMFLOAT4(1,0,0,1);
	//back red
	vertices[index].Position		= XMFLOAT3(-m_Width/2,  -m_Height/2, m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 0, 1);
	vertices[index++].Color	= XMFLOAT4(1,0,0,1);
	vertices[index].Position		= XMFLOAT3(-m_Width/2,   m_Height/2, m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 0, 1);
	vertices[index++].Color	= XMFLOAT4(1,0,0,1);
	vertices[index].Position		= XMFLOAT3( m_Width/2,  -m_Height/2, m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 0, 1);
	vertices[index++].Color	= XMFLOAT4(1,0,0,1);
	vertices[index].Position		= XMFLOAT3( m_Width/2,   m_Height/2, m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 0, 1);
	vertices[index++].Color	= XMFLOAT4(1,0,0,1);

	//left green
	vertices[index].Position		= XMFLOAT3(-m_Width/2,  -m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(-1, 0, 0);
	vertices[index++].Color	= XMFLOAT4(0,1,0,1);
	vertices[index].Position		= XMFLOAT3(-m_Width/2,   m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(-1, 0, 0);
	vertices[index++].Color	= XMFLOAT4(0,1,0,1);
	vertices[index].Position		= XMFLOAT3(-m_Width/2,  -m_Height/2,  m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(-1, 0, 0);
	vertices[index++].Color	= XMFLOAT4(0,1,0,1);
	vertices[index].Position		= XMFLOAT3(-m_Width/2,   m_Height/2,  m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(-1, 0, 0);
	vertices[index++].Color	= XMFLOAT4(0,1,0,1);

	//right green
	vertices[index].Position		= XMFLOAT3(m_Width/2,  -m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(1, 0, 0);
	vertices[index++].Color	= XMFLOAT4(0,1,0,1);
	vertices[index].Position		= XMFLOAT3(m_Width/2,   m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(1, 0, 0);
	vertices[index++].Color	= XMFLOAT4(0,1,0,1);
	vertices[index].Position		= XMFLOAT3(m_Width/2,  -m_Height/2,  m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(1, 0, 0);
	vertices[index++].Color	= XMFLOAT4(0,1,0,1);
	vertices[index].Position		= XMFLOAT3(m_Width/2,   m_Height/2,  m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(1, 0, 0);
	vertices[index++].Color	= XMFLOAT4(0,1,0,1);

	//top blue
	vertices[index].Position		= XMFLOAT3(-m_Width/2,  m_Height/2,  m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 1, 0);
	vertices[index++].Color	= XMFLOAT4(0,0,1,1);
	vertices[index].Position		= XMFLOAT3(-m_Width/2,  m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 1, 0);
	vertices[index++].Color	= XMFLOAT4(0,0,1,1);
	vertices[index].Position		= XMFLOAT3(m_Width/2,   m_Height/2,  m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 1, 0);
	vertices[index++].Color	= XMFLOAT4(0,0,1,1);
	vertices[index].Position		= XMFLOAT3(m_Width/2,   m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, 1, 0);
	vertices[index++].Color	= XMFLOAT4(0,0,1,1);

	//bottom blue
	vertices[index].Position		= XMFLOAT3(-m_Width/2, -m_Height/2,  m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, -1, 0);
	vertices[index++].Color	= XMFLOAT4(0,0,1,1);
	vertices[index].Position		= XMFLOAT3(-m_Width/2, -m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, -1, 0);
	vertices[index++].Color	= XMFLOAT4(0,0,1,1);
	vertices[index].Position		= XMFLOAT3(m_Width/2,  -m_Height/2,  m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, -1, 0);
	vertices[index++].Color	= XMFLOAT4(0,0,1,1);
	vertices[index].Position		= XMFLOAT3(m_Width/2,  -m_Height/2, -m_Depth/2);
	vertices[index].Normal	= XMFLOAT3(0, -1, 0);
	vertices[index++].Color	= XMFLOAT4(0,0,1,1);

	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = {0};

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof( VertexPosColNorm ) * NUMVERTICES;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = vertices;
	hr = pContext->GetDevice()->CreateBuffer( &bd, &initData, &m_pVertexBuffer );
	Debug::LogHResult(hr, L"Failed to Create VertexBuffer");

	//Indexbuffer
	DWORD indicesArr[NUMINDICES] = {
		0,1,2, 2,1,3,
		4,6,5, 5,6,7,
		8,10,9, 9,10,11,
		12,13,14, 14,13,15,
		16,18,17, 17,18,19,
		20,21,22, 22,21,23
	};

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth =  sizeof(DWORD) * NUMINDICES;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indicesArr;
	hr = pContext->GetDevice()->CreateBuffer( &bd, &initData, &m_pIndexBuffer );
	Debug::LogHResult(hr, L"Failed to Create IndexBuffer");
}

void CubePosColorNorm::Update(GameContext* pContext)
{

}

void CubePosColorNorm::Draw(GameContext* pContext)
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

	// Render a cube
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for(UINT p=0; p< techDesc.Passes;++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, deviceContext);
		deviceContext->DrawIndexed( NUMINDICES, 0, 0);
	}
}
