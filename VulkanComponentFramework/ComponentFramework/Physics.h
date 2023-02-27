#pragma once

#include "AABB.h"
#include "DynamicLinearMovement.h"

using namespace MATH;

namespace PHYSICS
{
	class Physics
	{
		


	public:
				
		static bool TestTwoAABB(AABB* b1, AABB* b2);
		static void Update(const float deltaTime, Ref<DynamicLinearMovement> DLM, Ref<AABB> cBox);
		
	};
}