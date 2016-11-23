#pragma once

#include "GameBase.h"

class GameScene;
using namespace std;

class SceneManager final
{
public:
	SceneManager(void);
	~SceneManager(void);

	static void AddGameScene(GameScene* scene);
	static void RemoveGameScene(GameScene* scene);
	static void SetActiveGameScene(wstring sceneName);
	static void NextScene();
	static void PreviousScene();
	static bool IsInputEnabled() { return m_InputEnabled; }

private:

	friend class GameBase;

	static void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static void Update();
	static void Draw();
	static void Destroy();
	static void WindowStateChanged(int state, bool active);

	static vector<GameScene*> m_vecScenes;
	static bool m_IsInitialized;
	static GameScene* m_ActiveScene, *m_NewActiveScene;
	static ID3D11Device* m_pDevice;
	static ID3D11DeviceContext* m_pContext;
	static bool m_InputEnabled;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SceneManager(const SceneManager& t);
	SceneManager& operator=(const SceneManager& t);
};

