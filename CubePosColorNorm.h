#pragma once
#include "GameObject.h"

class CubePosColorNorm: public GameObject
{
public:
	CubePosColorNorm(float width, float height, float depth);
	~CubePosColorNorm(void);

	virtual void Initialize(GameContext* pContext);
	virtual void Update(GameContext* pContext);
	virtual void Draw(GameContext* pContext);

private:

	float m_Width, m_Height, m_Depth;
	static const int NUMVERTICES = 24;
	static const int NUMINDICES = 36;

	ID3D11InputLayout*      m_pVertexLayout;
	ID3D11Buffer*           m_pVertexBuffer;
	ID3D11Buffer*           m_pIndexBuffer;
	ID3DX11Effect			*m_pEffect;
	ID3DX11EffectTechnique	*m_pTechnique;
	ID3DX11EffectMatrixVariable *m_pMatWorldViewProjVariable;
	ID3DX11EffectMatrixVariable *m_pMatWorldVariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	CubePosColorNorm(const CubePosColorNorm& t);
	CubePosColorNorm& operator=(const CubePosColorNorm& t);
};

