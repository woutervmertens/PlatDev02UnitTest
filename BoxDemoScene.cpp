#include "stdafx.h"
#include "BoxDemoScene.h"
#include "CubePosColorNorm.h"
#include "FreeCamera.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Debug.h"


BoxDemoScene::BoxDemoScene(void):
	GameScene(L"BoxDemoScene"),
	m_pCube0(nullptr),
	m_pCube1(nullptr),
	m_pCube2(nullptr),
	m_PrevCheck(0)
{
}


BoxDemoScene::~BoxDemoScene(void)
{
}

void BoxDemoScene::Initialize(GameContext* pContext)
{
	m_pCube0 = new CubePosColorNorm(5.f,5.f,5.f);
	m_pCube1 = new CubePosColorNorm(4.f,4.f,4.f);
	m_pCube1->Translate(6.f,0.f,0.f);
	m_pCube2 = new CubePosColorNorm(2.f,2.f,2.f);
	m_pCube2->Translate(0.f, 4.f, 0.f);

	AddGameObject(m_pCube0);
	m_pCube0->AddChild(m_pCube1);
	m_pCube1->AddChild(m_pCube2);
}

void BoxDemoScene::Update(GameContext* pContext)
{
	if(GetAsyncKeyState(VK_NUMPAD3)  & 0x8000)
	{
		SceneManager::SetActiveGameScene(L"GimbalRigScene");
	}
	else if(GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
	{
		SceneManager::SetActiveGameScene(L"CarouselScene");
	}

	float totSec = pContext->GetTimer()->GetTotalSeconds();

	m_pCube0->RotateEuler(0.f, totSec * 90.f, 0.f);
	m_pCube1->RotateEuler(totSec * 90.f, 0.f, 0.f);

	if(pContext->GetTimer()->GetTotalSeconds() >= m_PrevCheck)
	{
		++m_PrevCheck;
		wstringstream ss;
		ss<<L"FPS: "<<pContext->GetTimer()->GetFPS()<<"\n";
		Debug::LogInfo(ss.str());
	}
}

void BoxDemoScene::Draw(GameContext* pContext)
{
	//Nothing todo :)
}
