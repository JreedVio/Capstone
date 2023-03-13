#include "Physics.h"

using namespace PHYSICS;

PHYSICS::Physics::Physics(Component* parent_): Component(parent_)
{
}

bool Physics::TestTwoAABB(Ref<AABB> b1, Ref<AABB> b2)
{
	return Collision = AABB::TestAABB(b1, b2);	
}

void Physics::Update(const float deltaTime, Ref<DynamicLinearMovement> DLM, Ref<AABB> cBox)
{

	if (cBox)
	{
		cBox->Update(deltaTime);
	}
	if (DLM)
	{
		DLM->SetCollision(Collision);
		DLM->Update(deltaTime);
	}
}
