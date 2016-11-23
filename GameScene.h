#pragma once

#include "SceneManager.h"
#include "GameContext.h"


class GameObject;
class GameContext;
class BaseCamera;

class GameScene
{
public:
	GameScene(wstring sceneName);
	virtual ~GameScene(void);

	//GET
	GameContext* GetContext() const { return m_pGameContext; }

	//SET
	void SetCamera(BaseCamera* pCamera);

	//METHODS
	void AddGameObject(GameObject* pObject);
	void RemoveGameObject(GameObject* pObject);

protected:

	virtual void Initialize(GameContext* pContext) = 0;
	virtual void Update(GameContext* pContext) = 0;
	virtual void Draw(GameContext* pContext) = 0;
	virtual void SceneActivated() {}
	virtual void SceneDeactivated() {}

	vector<GameObject*> m_vecGameObjects;
	bool m_IsInitialized;

private:

	friend SceneManager;

	void RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void RootUpdate();
	void RootDraw();
	void RootSceneActivated();
	void RootSceneDeactivated();
	void RootWindowStateChanged(int state, bool active);

	GameContext* m_pGameContext;
	BaseCamera* m_pDefaultCamera;
	wstring m_SceneName;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameScene(const GameScene& t);
	GameScene& operator=(const GameScene& t);
};

