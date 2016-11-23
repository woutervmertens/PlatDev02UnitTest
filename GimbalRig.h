#pragma once
#include "GameObject.h"

class CylinderPosColorNorm;
class TorusPosColorNorm;
class CubePosColorNorm;

class GimbalRig: public GameObject
{
public:
	GimbalRig(void);
	~GimbalRig(void);

	virtual void Initialize(GameContext* pContext);
	virtual void Update(GameContext* pContext);
	virtual void Draw(GameContext* pContext);

private:

	CylinderPosColorNorm *m_pBase, *m_pPillar0, *m_pPillar1;
	TorusPosColorNorm *m_pOuterTorus, *m_pMiddleTorus, *m_pInnerTorus;
	CubePosColorNorm *m_pJoint0, *m_pJoint1;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GimbalRig(const GimbalRig& t);
	GimbalRig& operator=(const GimbalRig& t);
};

