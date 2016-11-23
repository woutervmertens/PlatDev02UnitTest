#include "stdafx.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "GameContext.h"
#include "GameTimer.h"


vector<GameScene*> SceneManager::m_vecScenes = vector<GameScene*>();
bool SceneManager::m_IsInitialized = false;
GameScene* SceneManager::m_ActiveScene = nullptr;
GameScene* SceneManager::m_NewActiveScene = nullptr;
ID3D11Device* SceneManager::m_pDevice = nullptr;
ID3D11DeviceContext* SceneManager::m_pContext = nullptr;
bool SceneManager::m_InputEnabled = true;

SceneManager::SceneManager(void)
{
}


SceneManager::~SceneManager(void)
{
}

void SceneManager::Destroy()
{
	for (GameScene* scene : m_vecScenes)
	{
		SafeDelete(scene);
	}
}

void SceneManager::AddGameScene(GameScene* scene)
{
	auto it = find(m_vecScenes.begin(), m_vecScenes.end(), scene);

	if (it == m_vecScenes.end())
	{
		m_vecScenes.push_back(scene);

		if (m_IsInitialized)
			scene->RootInitialize(m_pDevice, m_pContext);

		if (m_ActiveScene == nullptr && m_NewActiveScene == nullptr)
			m_NewActiveScene = scene;
	}
}

void SceneManager::RemoveGameScene(GameScene* scene)
{
	auto it = find(m_vecScenes.begin(), m_vecScenes.end(), scene);

	if (it != m_vecScenes.end())
	{
		m_vecScenes.erase(it);
	}
}

void SceneManager::NextScene()
{
	for (UINT i = 0; i < m_vecScenes.size(); ++i)
	{
		if (m_vecScenes[i] == m_ActiveScene)
		{
			auto nextScene = (++i) % m_vecScenes.size();
			m_NewActiveScene = m_vecScenes[nextScene];
			break;
		}
	}
}

void SceneManager::PreviousScene()
{
	for (UINT i = 0; i < m_vecScenes.size(); ++i)
	{
		if (m_vecScenes[i] == m_ActiveScene)
		{
			if (i == 0) i = m_vecScenes.size();
			--i;
			m_NewActiveScene = m_vecScenes[i];
			break;
		}
	}
}

void SceneManager::SetActiveGameScene(wstring sceneName)
{
	auto it = find_if(m_vecScenes.begin(), m_vecScenes.end(), [sceneName](GameScene* scene)
	{
		return (wcscmp(scene->m_SceneName.c_str(), sceneName.c_str()) == 0);
	});

	if (it != m_vecScenes.end())
	{
		m_NewActiveScene = *it;
	}
}

void SceneManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	for (GameScene* scene : m_vecScenes)
	{
		scene->RootInitialize(m_pDevice, m_pContext);
	}
}

void SceneManager::WindowStateChanged(int state, bool active)
{
	if (state == 0 && m_ActiveScene)
	{
		m_ActiveScene->RootWindowStateChanged(state, active);
	}
}

void SceneManager::Update()
{
	if (m_NewActiveScene != nullptr)
	{
		//Deactivate the current active scene
		if (m_ActiveScene != nullptr)
			m_ActiveScene->RootSceneDeactivated();

		//Set New Scene
		m_ActiveScene = m_NewActiveScene;
		m_NewActiveScene = nullptr;

		//Active the new scene and reset SceneTimer
		m_ActiveScene->RootSceneActivated();
	}

	if (m_ActiveScene != nullptr)
		m_ActiveScene->RootUpdate();
}

void SceneManager::Draw()
{
	if (m_ActiveScene != nullptr)
		m_ActiveScene->RootDraw();
}
