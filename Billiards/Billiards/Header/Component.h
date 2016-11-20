#pragma once
#include <DirectXMath.h>
#include <vector>
#include <typeindex>

using namespace std;
using namespace DirectX;

class GameObject;
class MeshData;

class Component
{
public:
	vector<type_index> id;
	Component(){}
	virtual ~Component() {}
	virtual void receive(int message) = 0;
};

class InputComponent : public Component
{
public:
	virtual ~InputComponent() {}
	virtual void Update() = 0;
};

class PhysicsComponent : public Component
{
public:
	GameObject* pGameObject;
	XMFLOAT3 prePos;		// 前フレームの位置
	XMFLOAT3 velocity;		// 移動ベクトル
	XMFLOAT3 acceleration;	// 加速度
	float	 mass;			// 質量

	PhysicsComponent(): Component(), pGameObject(nullptr),prePos(0.0f,0.0f,0.0f),velocity(0.0f, 0.0f, 0.0f),acceleration(0.0f, 0.0f, 0.0f),mass(0.0f)
	{
		id.push_back(typeid(this));
	}

	virtual ~PhysicsComponent(){}
	virtual void Update() = 0;
};

class GraphicsComponent : public Component
{
public:
	GameObject* pGameObject;
	MeshData* pMeshData;
	XMFLOAT4X4 world;
	int frame;

	GraphicsComponent() : Component(), pGameObject(nullptr), pMeshData(nullptr)
	{
		id.push_back(typeid(this));
	}

	virtual ~GraphicsComponent(){}
	virtual void Update() = 0;
};