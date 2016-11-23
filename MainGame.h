#pragma once
#include "GameBase.h"

class MainGame: public GameBase
{
public:
	MainGame(void);
	~MainGame(void);

protected: 

	virtual void OnPreparingGame(GameSettings& settings);
	virtual void Initialize();
	virtual LRESULT WindowsProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MainGame(const MainGame& t);
	MainGame& operator=(const MainGame& t);
};

