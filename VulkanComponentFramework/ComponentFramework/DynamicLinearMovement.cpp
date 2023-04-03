#include "DynamicLinearMovement.h"
#include "AABB.h"
#include "VMath.h"
#include "SceneManager.h"

PHYSICS::DynamicLinearMovement::DynamicLinearMovement()
{
}

PHYSICS::DynamicLinearMovement::DynamicLinearMovement(Component* parent_, const Ref<TransformComponent>& playerTrans) : Component(parent_),
TransformRef(playerTrans)
{
}

PHYSICS::DynamicLinearMovement::~DynamicLinearMovement()
{
	
}

void PHYSICS::DynamicLinearMovement::OnDestroy()
{
}

void PHYSICS::DynamicLinearMovement::Update(const float deltaTime)
{

	if (!TransformRef)
	{
		printf("MovementTransform is nullptr!, %s, %d\n", __FILE__, __LINE__);
		return;
	}	
	
	orient = TransformRef->GetOrientation();
	pos = TransformRef->GetPosition();

	UpdateFall(deltaTime);
	UpdateWall(deltaTime);	
}

void PHYSICS::DynamicLinearMovement::UpdateFall(const float deltaTime)
{
	if (!TransformRef)
	{
		printf("MovementTransform is nullptr!, %s, %d\n", __FILE__, __LINE__);
		return;
	}	
	
	pos += vel * deltaTime + accel * (0.5f * deltaTime * deltaTime);
	vel += accel * deltaTime;

	TransformRef->SetTransform(pos, orient);
	//printf("--------------------\n");
	//vel.print();
	//pos.print();
	
}

void PHYSICS::DynamicLinearMovement::UpdateWall(const float deltaTime)
{	
	auto Actor = SceneManager::GetInstance()->GetCurrentScene();

	/*auto wall = room->GetActor("Forward");
	
	auto status = wall->GetComponent<AABB>();
	if (!status)
	{
		return;
	}*/

	/*Vec3 penetration = PlayerRef->GetComponent<AABB>()->Penetration(status);
	
	if (penetration == Vec3())
	{
		return;
	}*/

	if (!TransformRef)
	{
		printf("MovementTransform is nullptr!, %s, %d\n", __FILE__, __LINE__);
		return;
	}
	
	// Right
	if (pos.x >= Actor->GetActor("Right")->GetComponent<TransformComponent>()->GetPosition().x - 1.5f)
	{
		TransformRef->SetTransform(Vec3(23.5f, pos.y, pos.z), orient);
		//printf("collision!\n");
	}
	// Left
	else if (pos.x <= Actor->GetActor("Left")->GetComponent<TransformComponent>()->GetPosition().x + 1.5f)
	{
		TransformRef->SetTransform(Vec3(-23.5f, pos.y, pos.z), orient);
		//printf("collision!\n");
	}
	
	// Front
	else if (pos.z <= Actor->GetActor("Forward")->GetComponent<TransformComponent>()->GetPosition().z + 1.5f)
	{
		TransformRef->SetTransform(Vec3(pos.x, pos.y, -23.5f), orient);
		//printf("collision!\n");
	}
	//Back
	else if (pos.z >= Actor->GetActor("Backward")->GetComponent<TransformComponent>()->GetPosition().z - 1.5f)
	{
		TransformRef->SetTransform(Vec3(pos.x, pos.y, 23.5f), orient);
		//printf("collision!\n");
	}

	//pos += penetration;
	//vel -= vel * VMath::dot( VMath::cross(VMath::normalize(penetration), Vec3(0, 1, 0)), VMath::cross(VMath::normalize(penetration), Vec3(0, 1, 0)) );
	
	//PlayerRef->GetComponent<TransformComponent>()->SetTransform(pos, orient);
}

