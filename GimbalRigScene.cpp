#include "stdafx.h"
#include "GimbalRigScene.h"
#include "GimbalRig.h"

GimbalRigScene::GimbalRigScene(void):
	GameScene(L"GimbalRigScene"),
	m_pGimbalRig(nullptr)
{
}


GimbalRigScene::~GimbalRigScene(void)
{
}

void GimbalRigScene::Initialize(GameContext* pContext)
{
	m_pGimbalRig = new GimbalRig();
	AddGameObject(m_pGimbalRig);
}

void GimbalRigScene::Update(GameContext* pContext)
{
	if(GetAsyncKeyState(VK_NUMPAD1)  & 0x8000)
	{
		SceneManager::SetActiveGameScene(L"BoxDemoScene");
	}
	else if(GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
	{
		SceneManager::SetActiveGameScene(L"CarouselScene");
	}
}
