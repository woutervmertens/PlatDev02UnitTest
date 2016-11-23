#pragma once

class GameContext;

class BaseCamera
{
public:
	BaseCamera(void);
	~BaseCamera(void);
	void SetQWERTY(bool isQuerty = true);

	XMFLOAT4X4 GetView() const { return m_View; }
	XMFLOAT4X4 GetProjection() const { return m_Projection; }
	XMFLOAT4X4 GetViewProjection() const { return m_ViewProjection; }

	XMFLOAT3 GetPosition() const { return m_Position; }
	XMFLOAT3 GetUp() const { return m_Up; }
	XMFLOAT3 GetRight() const { return m_Right; }
	XMFLOAT3 GetForward() const { return m_Forward; }
	XMFLOAT3 GetTarget() const { return m_Target; }

	virtual void BuildView();
	virtual void BuildProjection(float fov, float aspectRatio, float nearZ, float farZ);
	virtual void Update(GameContext* m_pContext) {};

protected:
	void BuildViewProjection();
	bool m_IsQWERTY;

	XMFLOAT3 m_Up, m_Right, m_Forward, m_Position, m_Target;
	XMFLOAT4X4 m_View, m_Projection, m_ViewProjection;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BaseCamera(const BaseCamera& t);
	BaseCamera& operator=(const BaseCamera& t);
};

