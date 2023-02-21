#include "Physics.h"

using namespace PHYSICS;

bool Physics::TestTwoAABB(const AABB& b1, const AABB& b2)
{
	return AABB::TestAABB(b1, b2);
}
