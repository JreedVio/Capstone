#include "AABB.h"
#include "Debug.h"

PHYSICS::AABB::AABB(Component* parent_, Ref<TransformComponent> ParentTransform_, Vec3 centre_, Vec3 scale_, Quaternion orientation_) : Component(parent_)
{
	Empty();

	scale = scale_;
	centre = centre_;
	orientation = orientation_;
	ParentTransform = ParentTransform_;

	SetBounds(scale, centre, orientation);
	Add(bounds);
}

PHYSICS::AABB::AABB(Ref<TransformComponent> ParentTransform_, Vec3 centre_, Vec3 scale_, Quaternion orientation_)
{
	Empty();

	scale = scale_;
	centre = centre_;
	orientation = orientation_;
	ParentTransform = ParentTransform_;

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

	printf("Collision!\n");
	
	return true;
}

void PHYSICS::AABB::Update(const float deltaTime_)
{
	if (!ParentTransform)
	{
		Debug::Error("ParentTransform is nullptr!", __FILE__, __LINE__);
	}
	else
	{
		SetCentre(ParentTransform->GetPosition() + Vec3(0.0f, 2.0f, 0.0f));
	}
	SetBounds(scale, centre, orientation);
	Add(bounds);
}
