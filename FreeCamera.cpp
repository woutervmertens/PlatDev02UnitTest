#include "stdafx.h"
#include "FreeCamera.h"
#include "GameContext.h"
#include "SceneManager.h"

FreeCamera::FreeCamera(void):
	m_TotalYaw(0),
	m_TotalPitch(0),
	m_Speed(30.0f),
	m_MouseDownPreviousFrame(false),
	m_IsQWERTY(true),
	m_Up('W'),
	m_Left('A'),
	m_Direction(XMFLOAT3(0,0,1.0f))
{
	BuildProjection(XM_PIDIV4, 1280.0f/720.0f,0.01f,100.0f);
}


FreeCamera::~FreeCamera(void)
{
}

void FreeCamera::Update(GameContext* pContext)
{
	if (!SceneManager::IsInputEnabled())
		return;

	float deltaTime = pContext->GetTimer()->GetElapsedSeconds();

	bool mouseDownNow = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	XMVECTOR direction = XMLoadFloat3(&m_Direction);

	if(mouseDownNow && !m_MouseDownPreviousFrame)
	{
		GetCursorPos(&m_PrevCursorPosition);
		m_MouseDownPreviousFrame = true;
	}
	else if(mouseDownNow)
	{
		POINT currCursorPosition;
		GetCursorPos(&currCursorPosition);

		float xDiff = (currCursorPosition.x - m_PrevCursorPosition.x)/200.0f;
		float yDiff = (currCursorPosition.y - m_PrevCursorPosition.y)/200.0f;

		m_TotalYaw += xDiff * XMConvertToRadians(45);
		m_TotalPitch += yDiff * XMConvertToRadians(45);

		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_TotalPitch, m_TotalYaw, 0);

		XMVECTOR forwardVec = {0,0,1.0f,0};
		direction = XMVector3TransformCoord(forwardVec,rotMat);

		m_PrevCursorPosition = currCursorPosition;
	}
	else m_MouseDownPreviousFrame = false;

	XMVECTOR up = {0,1,0,0};
	XMVECTOR right = XMVector3Cross(direction, up);
	XMVECTOR eyePos = XMLoadFloat3(&m_Position);

	if(GetAsyncKeyState(m_Up) & 0x8000)
		eyePos += direction * m_Speed * deltaTime;
	else if(GetAsyncKeyState('S') & 0x8000)
		eyePos -= direction * m_Speed * deltaTime;

	if(GetAsyncKeyState(m_Left) & 0x8000)
		eyePos += right * m_Speed * deltaTime;
	else if(GetAsyncKeyState('D') & 0x8000)
		eyePos -= right * m_Speed * deltaTime;
	
	XMStoreFloat3(&m_Direction, direction);
	XMStoreFloat3(&m_Position, eyePos);
	XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(eyePos, eyePos+direction, up));
	BuildViewProjection();
}

void FreeCamera::SetQWERTY(bool isQwerty)
{
	m_IsQWERTY=isQwerty;
	m_Left = m_IsQWERTY?'A':'Q';
	m_Up = m_IsQWERTY?'W':'Z';
}