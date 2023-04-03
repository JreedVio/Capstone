#pragma once

#include "Component.h"
#include "AABB.h"
#include "DynamicLinearMovement.h"

using namespace MATH;

namespace PHYSICS
{
	class Physics: public Component
	{
		
		bool Collision = false;
		

	public:
		
		Physics(Component* parent_);
		
		static bool TestTwoAABB(Ref<AABB> b1, Ref<AABB> b2);
		void Update(const float deltaTime, Ref<DynamicLinearMovement> DLM, Ref<AABB> cBox);

		static void RigidBodyMove(Ref<Actor> Player, Ref<Actor> Object, bool CollisionStatus, bool Dynamic = false);

		static void ApplyForce(Ref<DynamicLinearMovement> DynamicLinearMovementComp, const Vec3 force, const float mass);
		static void ApplyLinearForce(Ref<DynamicLinearMovement> DynamicLinearMovementComp, const Vec3 force);

		void UpdatePuzzle(const float deltaTime);

		virtual ~Physics() {};
		virtual bool OnCreate() { return true; }
		virtual void OnDestroy() {};
		virtual void Update(const float deltaTime_) {};
		virtual void Render()const {}
		
	};
}