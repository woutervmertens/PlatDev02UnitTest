#pragma once
#include "GameScene.h"
#include "Debug.h"

class GimbalRig;

class GimbalRigScene: public GameScene
{
public:
	GimbalRigScene(void);
	~GimbalRigScene(void);

protected:

	virtual void Initialize(GameContext* pContext);
	virtual void Update(GameContext* pContext);
	virtual void Draw(GameContext* pContext){};
	virtual void SceneActivated(){ Debug::LogInfo(L"GimbalRigScene Activated!"); }
	virtual void SceneDeactivated(){ Debug::LogInfo(L"GimbalRigScene Deactivated!"); }

private:

	GimbalRig *m_pGimbalRig;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GimbalRigScene(const GimbalRigScene& t);
	GimbalRigScene& operator=(const GimbalRigScene& t);
};

