#include "stdafx.h"
#include "GameObject.h"
#include "GameContext.h"


GameObject::GameObject(void):
	m_pScene(nullptr),
	m_pParent(nullptr),
	m_IsInitialized(false),
	m_vecChildren(vector<GameObject*>()),
	m_WorldMatrix(XMFLOAT4X4()),
	m_Position(XMFLOAT3(0,0,0)),
	m_Scale(XMFLOAT3(1,1,1)),
	m_Rotation(XMFLOAT3(0,0,0))
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}


GameObject::~GameObject(void)
{
	for(GameObject* pObject: m_vecChildren)
	{
		SafeDelete(pObject);
	}
}

GameScene* GameObject::GetScene() const
{
	if(m_pScene != nullptr)
		return m_pScene;

	if(m_pParent != nullptr)
		return m_pParent->GetScene();

	return nullptr;
}

void GameObject::AddChild(GameObject* pObject)
{
	auto it = std::find(m_vecChildren.begin(), m_vecChildren.end(), pObject);
	if(it == m_vecChildren.end())
	{
		m_vecChildren.push_back(pObject);
		pObject->m_pScene = nullptr;
		pObject->m_pParent = this;

		if(m_IsInitialized)
			pObject->RootInitialize(GameContext::GetInitContext());
	}
}

void GameObject::RemoveChild(GameObject* pObject)
{
	auto it = std::find(m_vecChildren.begin(), m_vecChildren.end(), pObject);
	if(it != m_vecChildren.end())
	{
		m_vecChildren.erase(it);
		pObject->m_pScene = nullptr;
		pObject->m_pParent = nullptr;
	}
}

void GameObject::Translate(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void GameObject::Rotate(float x, float y, float z)
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
}

void GameObject::RotateEuler(float x, float y, float z)
{
	m_Rotation.x = XMConvertToRadians(x);
	m_Rotation.y = XMConvertToRadians(y);
	m_Rotation.z = XMConvertToRadians(z);
}

void GameObject::Scale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void GameObject::RootInitialize(GameContext* pContext)
{
	if(m_IsInitialized)
		return;

	Initialize(pContext);

	for(GameObject* pObject: m_vecChildren)
	{
		pObject->RootInitialize(pContext);
	}

	m_IsInitialized = true;
}

void GameObject::RootUpdate(GameContext* pContext)
{
	Update(pContext);

	for(GameObject* pObject: m_vecChildren)
	{
		pObject->RootUpdate(pContext);
	}
}

void GameObject::RootDraw(GameContext* pContext)
{
	Draw(pContext);

	for(GameObject* pObject: m_vecChildren)
	{
		pObject->RootDraw(pContext);
	}
}

void GameObject::CalculateWorldMatrix(const XMMATRIX &parentWorld, bool updateChildren)
{
	XMMATRIX matTrans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMMATRIX matRot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	XMMATRIX matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	XMMATRIX matWorld = XMMatrixMultiply(matScale, matRot);
	matWorld = XMMatrixMultiply(matWorld, matTrans);
	matWorld = XMMatrixMultiply(matWorld, parentWorld);

	XMStoreFloat4x4(&m_WorldMatrix, matWorld);

	if(!updateChildren)
		return;

	for(GameObject* pObject: m_vecChildren)
	{
		pObject->CalculateWorldMatrix(matWorld, updateChildren);
	}
}