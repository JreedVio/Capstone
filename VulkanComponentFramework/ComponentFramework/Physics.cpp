#include "Physics.h"

using namespace PHYSICS;

PHYSICS::Physics::Physics(Component* parent_): Component(parent_)
{
}

bool Physics::TestTwoAABB(AABB* b1, AABB* b2)
{
	return Collision = AABB::TestAABB(b1, b2);	
}

void Physics::Update(const float deltaTime, Ref<DynamicLinearMovement> DLM, Ref<AABB> cBox)
{
	if (cBox)
	{
		cBox->Update(deltaTime);
	}
	if (!Collision && DLM)
	{
		DLM->Update(deltaTime);
	}
}
