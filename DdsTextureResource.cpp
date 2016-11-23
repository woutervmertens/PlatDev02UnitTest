#include "stdafx.h"
#include "DdsTextureResource.h"


DdsTextureResource::DdsTextureResource(ID3D11Resource* pTexture, ID3D11ShaderResourceView* pTextureShaderResourceView):
	m_pTexture(pTexture),
	m_pTextureShaderResourceView(pTextureShaderResourceView)
{
}


DdsTextureResource::~DdsTextureResource(void)
{
	SafeRelease(m_pTexture);
	SafeRelease(m_pTextureShaderResourceView);
}
