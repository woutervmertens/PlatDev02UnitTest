#pragma once


struct VertexPosCol
{
	VertexPosCol(XMFLOAT3 pos, XMFLOAT4 col):
		Position(pos),
		Color(col)
	{}

	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

struct VertexPosColNorm
{
	VertexPosColNorm(){}
	VertexPosColNorm(XMFLOAT3 pos, XMFLOAT4 col, XMFLOAT3 norm):
		Position(pos),
		Color(col),
		Normal(norm)
	{}	

	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT3 Normal;
};

struct VertexPosColNormTex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
};

struct VertexPosNormTex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
};