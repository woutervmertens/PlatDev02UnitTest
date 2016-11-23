#pragma once
// Windows Header Files:
#include <windows.h>
#include <string>
using namespace std;

#include "D3DUtil.h"

#include "Gamebase.h"

//Class Forward Declarations
class TrianglePos2D;
class TrianglePosCol2D;

class GP2_RenderingPipeline  final : public GameBase
{
public:
	GP2_RenderingPipeline(void);
	virtual ~GP2_RenderingPipeline(void);

private:
	virtual HRESULT GameInitialize();
	virtual void	GameUpdate();

	//Triangles
	TrianglePos2D *m_pTriangle1, *m_pTriangle2, *m_pTriangle3;
	TrianglePosCol2D *m_pColoredTriangle1, *m_pColoredTriangle2, *m_pColoredTriangle3;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GP2_RenderingPipeline(const GP2_RenderingPipeline& t);
	GP2_RenderingPipeline& operator=(const GP2_RenderingPipeline& t);
};

