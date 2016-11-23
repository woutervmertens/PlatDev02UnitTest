#pragma once


#include "GameContext.h"
#include <vector>

class GameScene;
using namespace std;

class GameObject
{
public:
	GameObject(void);
	virtual ~GameObject(void);

	XMFLOAT3 GetPosition() const { return m_Position; }
	XMFLOAT3 GetScale() const { return m_Scale; }
	XMFLOAT3 GetRotation() const { return m_Rotation; }
	XMFLOAT4X4 GetWorldMatrix() const { return m_WorldMatrix; }

	GameScene* GetScene() const;
	GameObject* GetParent() const { return m_pParent; }

	void AddChild(GameObject* pObject);
	void RemoveChild(GameObject* pObject);

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void RotateEuler(float x, float y, float z);
	void Scale(float x, float y, float z);

	virtual void Initialize(GameContext* pContext) = 0;
	virtual void Draw(GameContext* pContext) = 0;
	virtual void Update(GameContext* pContext) = 0;

protected:

	bool m_IsInitialized;
	vector<GameObject*> m_vecChildren;

	XMFLOAT4X4 m_WorldMatrix;
	XMFLOAT3 m_Position, m_Scale, m_Rotation;

private:
	friend GameScene;

	void RootInitialize(GameContext* pContext);
	void RootUpdate(GameContext* pContext);
	void RootDraw(GameContext* pContext);
	void CalculateWorldMatrix(const XMMATRIX &parentWorld, bool updateChildren = true);

	GameScene* m_pScene;
	GameObject* m_pParent;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameObject(const GameObject& t);
	GameObject& operator=(const GameObject& t);
};

