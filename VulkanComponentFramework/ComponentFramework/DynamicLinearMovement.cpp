#include "DynamicLinearMovement.h"
#include "AABB.h"
#include "VMath.h"


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
	//vel.print();
	//accel.print();
	if (!collision)
	{
		accel = Gravity / mass;	
		//accel.print();
	}
	else
	{
		accel = Vec3();
		vel.y = 0.0f;
	}
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
	
}

void PHYSICS::DynamicLinearMovement::UpdateWall(const float deltaTime)
{	
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
	if (pos.x >= 23.5f)
	{
		TransformRef->SetTransform(Vec3(23.5f, pos.y, pos.z), orient);
		printf("collision!\n");
	}
	// Left
	else if (pos.x <= -23.5f)
	{
		TransformRef->SetTransform(Vec3(-23.5f, pos.y, pos.z), orient);
		printf("collision!\n");
	}
	
	// Front
	else if (pos.z <= -23.5f)
	{
		TransformRef->SetTransform(Vec3(pos.x, pos.y, -23.5f), orient);
		printf("collision!\n");
	}
	//Back
	else if (pos.z >= 23.5f)
	{
		TransformRef->SetTransform(Vec3(pos.x, pos.y, 23.5f), orient);
		printf("collision!\n");
	}

	//pos += penetration;
	//vel -= vel * VMath::dot( VMath::cross(VMath::normalize(penetration), Vec3(0, 1, 0)), VMath::cross(VMath::normalize(penetration), Vec3(0, 1, 0)) );
	
	//PlayerRef->GetComponent<TransformComponent>()->SetTransform(pos, orient);
}

