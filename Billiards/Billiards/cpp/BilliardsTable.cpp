#include "../Header/BilliardsTable.h"
#include "../Header/ResourceManager.h"
#include "../Header/Collider.h"
#include "../Header/BilliardsTableComponents.h"
#include "../Header/MeshData.h"
#include "../Header/Pocket.h"

BilliardsTable::BilliardsTable() : GameObject(), pMesh(nullptr),pPhysicsComponent(nullptr),pGraphicsComponent(nullptr), pCollider(nullptr)
{
	SetName("Table");

	SetTag("Table");

	SetLayer("Table");
	//pMesh = ResourceManager::Instance().GetResource("Table", "Resource/billiardsTableCollider.fbx");
	ResourceManager::Instance().GetResource(&pMesh, "Resource/BilliardsTableModel.fbx");
	//pMesh = ResourceManager::Instance().GetResource("Table", "Resource/goblin2.fbx");

	pPhysicsComponent = NEW BilliardsTablePhysics(this);

	pGraphicsComponent = NEW BilliardsTableGraphics(this,pMesh);
	
	pCollider = NEW MeshCollider();
	pCollider->Create(this,Mesh,"Resource/billiardsTableCollider.fbx" ,140.0f);

	SetWorld();

	for (int i = 0; i < POCKET_COUNT; ++i)
	{
		Create<Pocket>(i + 1,-130.0f + (130.0f*(i % 3)),20.0f,-70.0f + (140.0f*(i % 2)));
	}
	
}

BilliardsTable::~BilliardsTable()
{
	pCollider->Destroy();
	SAFE_DELETE(pPhysicsComponent);
	SAFE_DELETE(pGraphicsComponent);
}

void BilliardsTable::Update()
{
	pPhysicsComponent->Update();

	pGraphicsComponent->Update();

	pCollider->Update();
}