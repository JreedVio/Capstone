#pragma once

#include "Component.h"
#include "AABB.h"
#include "DynamicLinearMovement.h"

using namespace MATH;

namespace PHYSICS
{
	class Physics {

	public:
		
		static bool TestTwoAABB(Ref<AABB> b1, Ref<AABB> b2);
		static void RigidBodyMove(Ref<Actor> Player, Ref<Actor> Object, bool CollisionStatus, bool Dynamic = false);
		static void PlayerWallResponse(Ref<Actor> Player, Ref<Actor> Wall);

		static void ApplyForce(Ref<DynamicLinearMovement> DynamicLinearMovementComp, const Vec3 force, const float mass);
		static void ApplyLinearForce(Ref<DynamicLinearMovement> DynamicLinearMovementComp, const Vec3 force);
	
	};
}