#include "AABB.h"

PHYSICS::AABB::AABB(Component* parent_, Vec3 centre_, Vec3 scale_, Quaternion orientation_) : Component(parent_)
{

	Empty();

	scale = scale_;
	centre = centre_;
	orientation = orientation_;

	SetBounds(scale, centre, orientation);
	Add(bounds);
}

PHYSICS::AABB::AABB(Vec3 centre_, Vec3 scale_, Quaternion orientation_)
{
	Empty();

	scale = scale_;
	centre = centre_;
	orientation = orientation_;

	SetBounds(scale, centre, orientation);
	Add(bounds);
}

PHYSICS::AABB::~AABB()
{
}

void PHYSICS::AABB::OnDestroy()
{
}

void PHYSICS::AABB::SetBounds(const Vec3 scale_, const Vec3 centre_, const Quaternion orient_)
{
	// bottom left front
	bounds[0] = orient_ * (centre_ + Vec3(-scale_.x, -scale_.y, scale_.z));

	// top left front
	bounds[1] = orient_ * (centre_ + Vec3(-scale_.x, scale_.y, scale_.z));

	// top right front
	bounds[2] = orient_ * (centre_ + Vec3(scale_.x, scale_.y, scale_.z));

	// bottom right front
	bounds[3] = orient_ * (centre_ + Vec3(scale_.x, -scale_.y, scale_.z));

	// bottom left back
	bounds[4] = orient_ * (centre_ + Vec3(-scale_.x, -scale_.y, -scale_.z));

	// top left back				    
	bounds[5] = orient_ * (centre_ + Vec3(-scale_.x, scale_.y, -scale_.z));

	// top right back
	bounds[6] = orient_ * (centre_ + Vec3(scale_.x, scale_.y, -scale_.z));

	// bottom right back
	bounds[7] = orient_ * (centre_ + Vec3(scale_.x, -scale_.y, -scale_.z));

}

void PHYSICS::AABB::Add(const Vec3 bounds_[])
{
	
	min.x = bounds_[0].x;
	max.x = bounds_[6].x;			

	min.y = bounds_[0].y;
	max.y = bounds_[6].y;

	min.z = bounds_[4].z;
	max.z = bounds_[1].z;		
	
}

bool PHYSICS::AABB::TestAABB(AABB* b1, AABB* b2)
{
	if (b1->min.x >= b2->max.x) return false;	
	if (b1->max.x <= b2->min.x) return false;

	if (b1->min.y >= b2->max.y) return false;
	if (b1->max.y <= b2->min.y) return false;

	if (b1->min.z >= b2->max.z) return false;
	if (b1->max.z <= b2->min.z) return false;
	
	return true;
}

void PHYSICS::AABB::Update(const float deltaTime_)
{

	SetBounds(scale, centre, orientation);
	Add(bounds);

}
