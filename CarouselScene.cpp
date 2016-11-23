#include "stdafx.h"
#include "CarouselScene.h"
#include "CylinderPosColorNorm.h"
#include "CubePosColorNorm.h"
#include "SceneManager.h"

CarouselScene::CarouselScene(void):
	GameScene(L"CarouselScene"),
	m_pBase(nullptr),
	m_Discs(vector<GameObject*>())
{
}


CarouselScene::~CarouselScene(void)
{
}

void CarouselScene::Initialize(GameContext* pContext)
{
	m_pBase = new CylinderPosColorNorm(20,20,0.75f,(XMFLOAT4)Colors::Yellow);

	float boxSize = 2.f;
	float discSize = 6.f;

	for(UINT i = 0; i < 3; ++i)
	{
		float discStep = (XM_2PI/3.f)*i;
		auto disc = new CylinderPosColorNorm(discSize,10,1.25f,(XMFLOAT4)Colors::Orange);
		disc->Translate(12 * cos(discStep),0.75f,12 * sin(discStep));

		for(UINT j = 0; j < 3; ++j)
		{
			float step = (XM_2PI/3.f)*j;
			auto box = new CubePosColorNorm(boxSize,boxSize,boxSize);
			box->Translate((discSize-2.5f) * cos(step),1.25f,(discSize-2.5f) * sin(step));
			disc->AddChild(box);
		}

		m_Discs.push_back(disc);
		m_pBase->AddChild(disc);
	}

	AddGameObject(m_pBase);
}

void CarouselScene::Update(GameContext* pContext)
{
	if(GetAsyncKeyState(VK_NUMPAD3)  & 0x8000)
	{
		SceneManager::SetActiveGameScene(L"GimbalRigScene");
	}
	else if(GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
	{
		SceneManager::SetActiveGameScene(L"BoxDemoScene");
	}


	float totSec = pContext->GetTimer()->GetTotalSeconds();

	m_pBase->RotateEuler(0,totSec*90, 0);
	for(auto obj:m_Discs)
	{
		obj->RotateEuler(0, -totSec*90, 0);
	}
}
