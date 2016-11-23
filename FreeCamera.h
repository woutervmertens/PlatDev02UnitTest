#pragma once
#include "BaseCamera.h"

class FreeCamera: public BaseCamera
{
public:
	FreeCamera(void);
	~FreeCamera(void);

	virtual void Update(GameContext* m_pContext);
	void SetQWERTY(bool isQuerty=true);

protected:

private:
	POINT m_PrevCursorPosition;
	float m_TotalYaw, m_TotalPitch;
	float m_Speed;
	bool m_MouseDownPreviousFrame;
	XMFLOAT3 m_Direction;

	bool m_IsQWERTY;
	//holds acii wasd or zqsd
	char m_Up, m_Left;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	FreeCamera(const FreeCamera& t);
	FreeCamera& operator=(const FreeCamera& t);
};

