#include "stdafx.h"
#include "ContentManager.h"
#include "EffectLoader.h"
#include "DdsTextureLoader.h"

vector<BaseLoader*> ContentManager::m_Loaders = vector<BaseLoader*>();
ID3D11Device* ContentManager::m_pDevice = nullptr;
bool ContentManager::m_IsInitialized = false;

ContentManager::ContentManager()
{
}


ContentManager::~ContentManager(void)
{
}

void ContentManager::Unload()
{
	for(BaseLoader *ldr:m_Loaders)
	{	
		ldr->Unload();
		SafeDelete(ldr);
	}

	m_Loaders.clear();
}

void ContentManager::Initialize(ID3D11Device* pDevice)
{
	if(!m_IsInitialized)
	{
		m_pDevice = pDevice;
		m_IsInitialized = true;
		AddLoader(new EffectLoader());
		AddLoader(new DdsTextureLoader());
	}
}

void ContentManager::AddLoader(BaseLoader* loader)
{ 
	for(BaseLoader *ldr:m_Loaders)
	{	
		if(ldr->GetType()==loader->GetType())
		{
			SafeDelete(loader);
			return;
		}
	}

	m_Loaders.push_back(loader);
	loader->SetDevice(m_pDevice);
}
