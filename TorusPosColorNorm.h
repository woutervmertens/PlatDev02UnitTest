#pragma once

#include "GameObject.h"

class TorusPosColorNorm: public GameObject
{
public:
	TorusPosColorNorm(float majorRadius, UINT majorRadiusSteps, float minorRadius, UINT minorRadiusSteps, XMFLOAT4 color = (XMFLOAT4)Colors::Green);
	~TorusPosColorNorm(void);

	virtual void Initialize(GameContext* pContext);
	virtual void Update(GameContext* pContext);
	virtual void Draw(GameContext* pContext);

private:

	float m_MajorRadius, m_MinorRadius;
	UINT m_MajorRadiusSteps, m_MinorRadiusSteps, m_NumVerts, m_NumIndices;
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
	TorusPosColorNorm(const TorusPosColorNorm& t);
	TorusPosColorNorm& operator=(const TorusPosColorNorm& t);
};

