#include "GP2_RenderingPipeline.h"
#include <sstream>
#include <iostream>
#include <vector>

#include "TrianglePos2D.h"
#include "TrianglePosCol2D.h"

GP2_RenderingPipeline::GP2_RenderingPipeline(void)
	:m_pTriangle1(nullptr)
	,m_pTriangle2(nullptr)
	,m_pTriangle3(nullptr)
	,m_pColoredTriangle1(nullptr)
	,m_pColoredTriangle2(nullptr)
	,m_pColoredTriangle3(nullptr)

{

}

GP2_RenderingPipeline::~GP2_RenderingPipeline(void)
{
	delete m_pTriangle1;
	delete m_pTriangle2;
	delete m_pTriangle3;
	delete m_pColoredTriangle1;
	delete m_pColoredTriangle2;
	delete m_pColoredTriangle3;
}

HRESULT GP2_RenderingPipeline::GameInitialize()
{
	HRESULT hr;

	m_pTriangle1 = new TrianglePos2D(m_pDevice);
	hr = m_pTriangle1->Create(XMFLOAT3( -0.5f, 0.0f, 0.5f ),XMFLOAT3( 0.0f, -1.0f, 0.5f ),XMFLOAT3( -1.0f, -1.0f, 0.5f ));
	if(FAILED(hr))
	{
		MessageBox (NULL, _T("Create TrianglePos2D Failed"), _T("TrianglePos2D"), MB_ICONERROR) ;
		exit(-1);
	}

	m_pTriangle2 = new TrianglePos2D(m_pDevice);
	hr = m_pTriangle2->Create(XMFLOAT3( 0.5f, 0.0f, 0.99f ),XMFLOAT3( 1.0f, -1.0f, 0.99f ),XMFLOAT3( -0.0f, -1.0f, 0.99f ));
	if(FAILED(hr))
	{
		MessageBox (NULL, _T("Create TrianglePos2D Failed"), _T("TrianglePos2D"), MB_ICONERROR) ;
		exit(-1);
	}
	m_pTriangle3 = new TrianglePos2D(m_pDevice);
	hr = m_pTriangle3->Create(XMFLOAT3( 0.0f, 1.0f, 0.99f ),XMFLOAT3( 0.5f, -0.0f, 0.99f ),XMFLOAT3( -0.5f, -0.0f, 0.99f ));
	if(FAILED(hr))
	{
		MessageBox (NULL, _T("Create TrianglePos2D Failed"), _T("TrianglePos2D"), MB_ICONERROR) ;
		exit(-1);
	}

	m_pColoredTriangle1 = new TrianglePosCol2D(m_pDevice);
	hr = m_pColoredTriangle1->Create(XMFLOAT3( -1.0f, 1.0f, 0.99f ),XMFLOAT3( 0.0f, 1.0f, 0.99f ),XMFLOAT3( -1.0f, -1.0f, 0.99f ));
	if(FAILED(hr))
	{
		MessageBox (NULL, _T("Create TrianglePos2D Failed"), _T("TrianglePos2D"), MB_ICONERROR) ;
		exit(-1);
	}

	m_pColoredTriangle2 = new TrianglePosCol2D(m_pDevice);
	hr = m_pColoredTriangle2->Create(XMFLOAT3( 1.0f, -1.0f, 0.99f ),XMFLOAT3( 0.0f, 1.0f, 0.99f ),XMFLOAT3( 1.0f, 1.0f, 0.99f ));
	if(FAILED(hr))
	{
		MessageBox (NULL, _T("Create TrianglePos2D Failed"), _T("TrianglePos2D"), MB_ICONERROR) ;
		exit(-1);
	}
	m_pColoredTriangle3 = new TrianglePosCol2D(m_pDevice);
	hr = m_pColoredTriangle3->Create(XMFLOAT3( -0.5f, 0.0f, 0.99f ),XMFLOAT3( 0.5f, 0.0f, 0.99f ),XMFLOAT3( 0.0f, -1.0f, 0.99f ));
	if(FAILED(hr))
	{
		MessageBox (NULL, _T("Create TrianglePos2D Failed"), _T("TrianglePos2D"), MB_ICONERROR) ;
		exit(-1);
	}

	return S_OK;
}

void GP2_RenderingPipeline::GameUpdate()
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView,DirectX::Colors::DarkGray);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Paint the triangle
	if(m_pTriangle1)m_pTriangle1->Paint(m_pDeviceContext);
	if(m_pTriangle2)m_pTriangle2->Paint(m_pDeviceContext);
	if(m_pTriangle3)m_pTriangle3->Paint(m_pDeviceContext);

	if(m_pColoredTriangle1)m_pColoredTriangle1->Paint(m_pDeviceContext);
	if(m_pColoredTriangle2)m_pColoredTriangle2->Paint(m_pDeviceContext);
	if(m_pColoredTriangle3)m_pColoredTriangle3->Paint(m_pDeviceContext);


	//increment the frame counter
	++m_NumFrames;

	// swap front and backbuffer: Display next rendered frame
	//m_pSwapChain->Present(m_VBLWait?1:0,0);

	return ;
}

