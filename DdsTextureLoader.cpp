#include "stdafx.h"
#include "DdsTextureLoader.h"


DdsTextureLoader::DdsTextureLoader(void)
{
}


DdsTextureLoader::~DdsTextureLoader(void)
{
}

DdsTextureResource* DdsTextureLoader::LoadContent(wstring assetFile)
{
	ID3D11Resource* pTexture;
	ID3D11ShaderResourceView* pShaderResourceView;

	DDS_ALPHA_MODE amode = DDS_ALPHA_MODE_PREMULTIPLIED;
	auto hr = CreateDDSTextureFromFile(m_pDevice, assetFile.c_str(), &pTexture, &pShaderResourceView,0,&amode);
	
	wstringstream ss;
	ss<<"DdsTextureLoader: Failed to CreateDDsTextureFromFile!\nPath: ";
	ss<<assetFile;
	if(Debug::LogHResult(hr, ss.str()))
		return nullptr;

	return new DdsTextureResource(pTexture, pShaderResourceView);
}

void DdsTextureLoader::Destroy(DdsTextureResource* objToDestroy)
{
	SafeDelete(objToDestroy);
}
