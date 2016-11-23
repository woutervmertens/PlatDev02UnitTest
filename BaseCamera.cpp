#include "stdafx.h"
#include "BaseCamera.h"


BaseCamera::BaseCamera(void) :
	m_View(XMFLOAT4X4()),
	m_Projection(XMFLOAT4X4()),
	m_ViewProjection(XMFLOAT4X4()),
	m_Up(XMFLOAT3(0, 1, 0)),
	m_Right(XMFLOAT3(1, 0, 0)),
	m_Forward(XMFLOAT3(0, 0, 1)),
	m_Position(XMFLOAT3(0, 2, -5)),
	m_Target(XMFLOAT3(0, 0, 0)),
	m_IsQWERTY(true)
{
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
}


BaseCamera::~BaseCamera(void)
{
}

void BaseCamera::BuildView()
{
	auto pos = XMLoadFloat3(&m_Position);
	auto target = pos + XMLoadFloat3(&m_Forward);
	XMStoreFloat3(&m_Target, target);

	XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(pos, target, XMLoadFloat3(&m_Up)));
	BuildViewProjection();
}

void BaseCamera::BuildProjection(float fov, float aspectRatio, float nearZ, float farZ)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ));
	BuildViewProjection();
}

inline void BaseCamera::BuildViewProjection()
{
	XMMATRIX view, proj;
	view = XMLoadFloat4x4(&m_View);
	proj = XMLoadFloat4x4(&m_Projection);
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixMultiply(view, proj));
}

void BaseCamera::SetQWERTY(bool isQwerty)
{
	m_IsQWERTY = isQwerty;
}