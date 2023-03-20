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
		
		bool TestTwoAABB(Ref<AABB> b1, Ref<AABB> b2);
		void Update(const float deltaTime, Ref<DynamicLinearMovement> DLM, Ref<AABB> cBox);

		void UpdatePuzzle(const float deltaTime);

		virtual ~Physics() {};
		virtual bool OnCreate() { return true; }
		virtual void OnDestroy() {};
		virtual void Update(const float deltaTime_) {};
		virtual void Render()const {}
		
	};
}