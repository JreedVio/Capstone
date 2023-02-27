#include "Physics.h"

using namespace PHYSICS;

bool Physics::TestTwoAABB(AABB* b1, AABB* b2)
{
	return AABB::TestAABB(b1, b2);	
}

void Physics::Update(const float deltaTime, Ref<DynamicLinearMovement> DLM, Ref<AABB> cBox)
{		
	//DLM->Update(deltaTime);
	cBox->Update(deltaTime);
}
