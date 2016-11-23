#pragma once

#include "GameTimer.h"
#include "BaseCamera.h"

class GameContext final
{
public:
	GameContext(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~GameContext(void);

	BaseCamera* GetCamera() const {return m_pCamera;}
	void SetCamera(BaseCamera* pCamera);

	GameTimer* GetTimer() const {return m_pTimer;}
	ID3D11Device* GetDevice() const { return m_pDevice; }
	ID3D11DeviceContext* GetDeviceContext() const {return m_pDeviceContext; }

	//Only use the Device and/or DeviceContext from this Init GameContext object!
	static GameContext* GetInitContext() { return m_InitContext; }

private:

	GameTimer* m_pTimer;
	BaseCamera* m_pCamera;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	static GameContext* m_InitContext;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameContext(const GameContext& t);
	GameContext& operator=(const GameContext& t);
};

