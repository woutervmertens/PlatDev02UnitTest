#include "stdafx.h"
#include "GameScene.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "Debug.h"


GameScene::GameScene(wstring sceneName) :
	m_vecGameObjects(vector<GameObject*>()),
	m_IsInitialized(false),
	m_pGameContext(nullptr),
	m_pDefaultCamera(nullptr),
	m_SceneName(sceneName)
{
}


GameScene::~GameScene(void)
{
	for (GameObject* pObject : m_vecGameObjects)
	{
		SafeDelete(pObject);
	}

	SafeDelete(m_pGameContext);
	SafeDelete(m_pDefaultCamera);
}

void GameScene::AddGameObject(GameObject* pObject)
{
	auto it = std::find(m_vecGameObjects.begin(), m_vecGameObjects.end(), pObject);
	if (it == m_vecGameObjects.end())
	{
		m_vecGameObjects.push_back(pObject);
		pObject->m_pScene = this;
		pObject->m_pParent = nullptr;

		if (m_IsInitialized)
			pObject->RootInitialize(m_pGameContext);
	}
}

void GameScene::RemoveGameObject(GameObject* pObject)
{
	auto it = std::find(m_vecGameObjects.begin(), m_vecGameObjects.end(), pObject);
	if (it != m_vecGameObjects.end())
	{
		m_vecGameObjects.erase(it);
		pObject->m_pScene = nullptr;
		pObject->m_pParent = nullptr;
	}
}

void GameScene::SetCamera(BaseCamera* pCamera)
{
	if (!pCamera)
		pCamera = m_pDefaultCamera;

	m_pGameContext->SetCamera(pCamera);
}

void GameScene::RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	if (m_IsInitialized)
		return;

	//Initialize Context
	m_pGameContext = new GameContext(pDevice, pDeviceContext);
	m_pGameContext->GetTimer()->Reset();
	m_pGameContext->GetTimer()->Stop();

	//Default Camera
	m_pDefaultCamera = new FreeCamera();
	m_pGameContext->SetCamera(m_pDefaultCamera);

	Initialize(m_pGameContext);

	for (GameObject* pObject : m_vecGameObjects)
	{
		pObject->RootInitialize(m_pGameContext);
	}

	m_IsInitialized = true;
}

void GameScene::RootUpdate()
{
	m_pGameContext->GetTimer()->Update();

	if (m_pGameContext->GetCamera())
		m_pGameContext->GetCamera()->Update(m_pGameContext);

	Update(m_pGameContext);

	for (GameObject* pObject : m_vecGameObjects)
	{
		pObject->RootUpdate(m_pGameContext);
	}

	for (GameObject* pObject : m_vecGameObjects)
	{
		pObject->CalculateWorldMatrix(XMMatrixIdentity()); //or inside GameObject itself??
	}
}

void GameScene::RootDraw()
{
	Draw(m_pGameContext);

	for (GameObject* pObject : m_vecGameObjects)
	{
		pObject->RootDraw(m_pGameContext);
	}

	Debug::Draw(m_pGameContext);
}

void GameScene::RootWindowStateChanged(int state, bool active)
{
	//TIMER
	if (state == 0)
	{
		if (active)m_pGameContext->GetTimer()->Start();
		else m_pGameContext->GetTimer()->Stop();
	}
}

void GameScene::RootSceneActivated()
{
	//Stop Timer
	m_pGameContext->GetTimer()->Start();

	SceneActivated();
}

void GameScene::RootSceneDeactivated()
{
	//Start Timer
	m_pGameContext->GetTimer()->Stop();

	SceneDeactivated();
}
