#include "Physics.h"
#include "SceneManager.h"
#include <memory>

using namespace PHYSICS;

PHYSICS::Physics::Physics(Component* parent_): Component(parent_)
{
	
}

bool Physics::TestTwoAABB(Ref<AABB> b1, Ref<AABB> b2)
{
	if (!b1 || !b2)
	{
		//printf("either b1 or b2 is null!\n");
		return false;
	}
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

void PHYSICS::Physics::UpdatePuzzle(const float deltaTime)
{
	// only works if you don't need to go back to the first room
	static int status = 0;
	if (status == 0)
	{
		Ref<DoorActor> doorActor = std::dynamic_pointer_cast<DoorActor>(SceneManager::GetInstance()->GetCurrentScene()->GetActor("Door"));
		doorActor->SetIsOpened(!doorActor->GetIsOpened());
		status++;
	}
}

