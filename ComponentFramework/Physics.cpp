#include "Physics.h"
#include "SceneManager.h"
#include <memory>

using namespace PHYSICS;

bool Physics::TestTwoAABB(Ref<AABB> b1, Ref<AABB> b2)
{
	if (!b1 || !b2)
	{
		//printf("either b1 or b2 is null!\n");
		return false;
	}
	return AABB::TestAABB(b1, b2);	
}

void PHYSICS::Physics::RigidBodyMove(Ref<Actor> Player, Ref<Actor> Object, bool CollisionStatus, bool Dynamic)
{
	if (!Dynamic && Player && Object)
	{
		auto ObjectDLM = Object->GetComponent<DynamicLinearMovement>();		
		if (!ObjectDLM) return;
		if (CollisionStatus && ObjectDLM)
		{
			auto playerTrans = Player->GetComponent<TransformComponent>();			
			if (playerTrans->GetPosition().x > Object->GetComponent<AABB>()->GetMax().x)
			{
				Physics::ApplyLinearForce(ObjectDLM, Vec3(-5.0f, 0.0f, 0.0f));
			}
			else if (playerTrans->GetPosition().x < Object->GetComponent<AABB>()->GetMin().x)
			{
				Physics::ApplyLinearForce(ObjectDLM, Vec3(5.0f, 0.0f, 0.0f));
			}
			else if (playerTrans->GetPosition().z > Object->GetComponent<AABB>()->GetMax().z)
			{
				Physics::ApplyLinearForce(ObjectDLM, Vec3(0.0f, 0.0f, -5.0f));
			}
			else if (playerTrans->GetPosition().z < Object->GetComponent<AABB>()->GetMin().z)
			{
				Physics::ApplyLinearForce(ObjectDLM, Vec3(0.0f, 0.0f, 5.0f));
			}
		}
		else
		{
			ObjectDLM->SetAccel(Vec3(0.0f, ObjectDLM->GetAccel().y, 0.0f));
			ObjectDLM->SetVel(Vec3(0.0f, ObjectDLM->GetVel().y, 0.0f));
		}
	}
}

void PHYSICS::Physics::PlayerWallResponse(Ref<Actor> Player, Ref<Actor> Wall){
	Ref<AABB> wallCollision = Wall->GetComponent<AABB>();
	Ref<DynamicLinearMovement> playerMovement = Player->GetComponent<DynamicLinearMovement>();
	auto playerTrans = Player->GetComponent<TransformComponent>();
	Vec3 position = playerTrans->GetPosition();
	float vel = 0.7f;
	float offset = 1.5f;
	//Physics::ApplyLinearForce(playerMovement, -playerMovement->GetVel() * vel);
	if (playerTrans->GetPosition().x > wallCollision->GetMax().x)
	{
		Vec3 position_(wallCollision->GetMax().x + offset, position.y, position.z);
		playerTrans->SetTransform(position_, playerTrans->GetOrientation());
	}
	else if (playerTrans->GetPosition().x < wallCollision->GetMin().x)
	{
		Vec3 position_(wallCollision->GetMin().x - offset, position.y, position.z);
		playerTrans->SetTransform(position_, playerTrans->GetOrientation());
	}
	else if (playerTrans->GetPosition().z > wallCollision->GetMax().z)
	{
		Vec3 position_(position.x, position.y, wallCollision->GetMax().z + offset);
		playerTrans->SetTransform(position_, playerTrans->GetOrientation());
	}
	else if (playerTrans->GetPosition().z < wallCollision->GetMin().z)
	{
		Vec3 position_(position.x, position.y, wallCollision->GetMin().z - offset);
		playerTrans->SetTransform(position_, playerTrans->GetOrientation());
	}

}

void PHYSICS::Physics::ApplyForce(Ref<DynamicLinearMovement> DynamicLinearMovementComp, const Vec3 force, const float mass)
{
	if (!DynamicLinearMovementComp)
	{
		printf("ApplyForce Call: DynamicLinearMovement is null!\n");
		return;
	}
	DynamicLinearMovementComp->SetAccel(force * mass);
}

void PHYSICS::Physics::ApplyLinearForce(Ref<DynamicLinearMovement> DynamicLinearMovementComp, const Vec3 force)
{
	if (!DynamicLinearMovementComp)
	{
		printf("ApplyForce Call: DynamicLinearMovement is null!\n");
		return;
	}
	DynamicLinearMovementComp->SetVel(force);
}

//void PHYSICS::Physics::UpdatePuzzle(const float deltaTime)
//{
//	// only works if you don't need to go back to the first room
//	static int status = 0;
//	if (status == 0)
//	{
//		Ref<DoorActor> doorActor = std::dynamic_pointer_cast<DoorActor>(SceneManager::GetInstance()->GetCurrentScene()->GetActor("Door"));
//		doorActor->SetIsOpened(!doorActor->GetIsOpened());
//		status++;
//	}
//}

