#include "stdafx.h"
#include "GameContext.h"
#include "GameTimer.h"
#include "Debug.h"

GameContext* GameContext::m_InitContext = nullptr;

GameContext::GameContext(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext):
	m_pTimer(nullptr),
	m_pCamera(nullptr),
	m_pDevice(pDevice),
	m_pDeviceContext(pDeviceContext)
{
	m_pTimer = new GameTimer();

	if(m_InitContext == nullptr)
		m_InitContext = this;
}

void GameContext::SetCamera(BaseCamera* pCamera)
{
	if(!pCamera)
		Debug::LogWarning(L"GameContext::SetCamera(nullptr)!");

	m_pCamera = pCamera;
}

GameContext::~GameContext(void)
{
	SafeDelete(m_pTimer);
}

