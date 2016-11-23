#pragma once

#include "GameScene.h"
#include "Debug.h"

class CubePosColorNorm;

class BoxDemoScene: public GameScene
{
public:
	BoxDemoScene(void);
	~BoxDemoScene(void);

protected:

	virtual void Initialize(GameContext* pContext);
	virtual void Update(GameContext* pContext);
	virtual void Draw(GameContext* pContext);
	virtual void SceneActivated(){ Debug::LogInfo(L"BoxDemoScene Activated!"); }
	virtual void SceneDeactivated(){ Debug::LogInfo(L"BoxDemoScene Deactivated!"); }

private:

	CubePosColorNorm *m_pCube0, *m_pCube1, *m_pCube2;
	float m_PrevCheck;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BoxDemoScene(const BoxDemoScene& t);
	BoxDemoScene& operator=(const BoxDemoScene& t);
};

