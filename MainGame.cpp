#include "stdafx.h"
#include "MainGame.h"
#include "SceneManager.h"
#include "BoxDemoScene.h"
#include "GimbalRigScene.h"
#include "CarouselScene.h"

MainGame::MainGame(void)
{
}


MainGame::~MainGame(void)
{
}

void MainGame::OnPreparingGame(GameSettings& settings)
{
	settings.VBLWait = false;
	settings.Title = L"Graphics Programming 1 - W6 GP1Framework DEMO";
}

void MainGame::Initialize()
{
	SceneManager::AddGameScene(new BoxDemoScene());
	SceneManager::AddGameScene(new GimbalRigScene());
	SceneManager::AddGameScene(new CarouselScene());

	//SceneManager::SetActiveGameScene(L"CarouselScene");
}

LRESULT MainGame::WindowsProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	
	case WM_KEYUP:
	{
		if ((lParam & 0x80000000) != 0x80000000)
			return -1;

		//NextScene
		if (wParam == 'P')
		{
			SceneManager::NextScene();
			return 0;
		}
		//PreviousScene
		else if (wParam == 'O')
		{
			SceneManager::PreviousScene();
			return 0;
		}
		else if (wParam == 'I')
		{
			Debug::ToggleDebugRenderer();
			return 0;
		}
	}
	}

	return -1;
}
