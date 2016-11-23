#pragma once
#include "GameScene.h"
#include "Debug.h"

class CylinderPosColorNorm;

class CarouselScene: public GameScene
{
public:
	CarouselScene(void);
	~CarouselScene(void);

protected:

	virtual void Initialize(GameContext* pContext);
	virtual void Update(GameContext* pContext);
	virtual void Draw(GameContext* pContext){};
	virtual void SceneActivated(){ Debug::LogInfo(L"CarouselScene Activated!"); }
	virtual void SceneDeactivated(){ Debug::LogInfo(L"CarouselScene Deactivated!"); }

private:

	CylinderPosColorNorm *m_pBase;
	vector<GameObject*> m_Discs;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	CarouselScene(const CarouselScene& t);
	CarouselScene& operator=(const CarouselScene& t);
};

