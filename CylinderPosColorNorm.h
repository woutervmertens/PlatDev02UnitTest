#pragma once

#include "GameObject.h"

class CylinderPosColorNorm: public GameObject
{
public:
	CylinderPosColorNorm(float radius, UINT radiusSteps, float height, XMFLOAT4 color = (XMFLOAT4)Colors::Green);
	~CylinderPosColorNorm(void);

	virtual void Initialize(GameContext* pContext);
	virtual void Update(GameContext* pContext);
	virtual void Draw(GameContext* pContext);

private:

	float m_Radius, m_Height;
	UINT m_RadiusSteps, m_NumVerts, m_NumIndices;
	XMFLOAT4 m_Color;

	ID3D11InputLayout*      m_pVertexLayout;
	ID3D11Buffer*           m_pVertexBuffer;
	ID3D11Buffer*           m_pIndexBuffer;
	ID3DX11Effect			*m_pEffect;
	ID3DX11EffectTechnique	*m_pTechnique;
	ID3DX11EffectMatrixVariable *m_pMatWorldViewProjVariable;
	ID3DX11EffectMatrixVariable *m_pMatWorldVariable;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	CylinderPosColorNorm(const CylinderPosColorNorm& t);
	CylinderPosColorNorm& operator=(const CylinderPosColorNorm& t);
};

