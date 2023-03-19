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
	bounds[0] = (centre_ + orient_ * Vec3(-scale_.x, -scale_.y, scale_.z));

	// top left front
	bounds[1] = (centre_ + orient_ * Vec3(-scale_.x, scale_.y, scale_.z));

	// top right front
	bounds[2] = (centre_ + orient_ * Vec3(scale_.x, scale_.y, scale_.z));

	// bottom right front
	bounds[3] = (centre_ + orient_ * Vec3(scale_.x, -scale_.y, scale_.z));

	// bottom left back
	bounds[4] = (centre_ + orient_ * Vec3(-scale_.x, -scale_.y, -scale_.z));

	// top left back				    
	bounds[5] = (centre_ + orient_ * Vec3(-scale_.x, scale_.y, -scale_.z));

	// top right back
	bounds[6] = (centre_ + orient_ * Vec3(scale_.x, scale_.y, -scale_.z));

	// bottom right back
	bounds[7] = (centre_ + orient_ * Vec3(scale_.x, -scale_.y, -scale_.z));
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

Vec3 PHYSICS::AABB::ClosestPoint(Vec3 point) const
{
	Vec3 closest = point;
	closest.x = std::max(min.x, std::min(max.x, closest.x));
	closest.y = std::max(min.y, std::min(max.y, closest.y));
	closest.z = std::max(min.z, std::min(max.z, closest.z));
	return closest;
}

Vec3 PHYSICS::AABB::Penetration(Ref<AABB> b1) const
{
	if (!b1)
	{
		Debug::Error("Object does not have an AABB component!\n", __FILE__, __LINE__);
		return NULL;
	}

	Vec3 distance = b1->ClosestPoint(Vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2)) - Vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	Vec3 minOverlap = distance;
	Vec3 axis = Vec3();
	float overlap = std::abs(b1->max.x - min.x);
	if (overlap < minOverlap.x) {
		minOverlap.x = overlap;
		axis = Vec3(1, 0, 0);
	}
	overlap = std::abs(b1->min.x - max.x);
	if (overlap < minOverlap.x) {
		minOverlap.x = overlap;
		axis = Vec3(-1, 0, 0);
	}
	overlap = std::abs(b1->min.y - max.y);
	if (overlap < minOverlap.y) {
		minOverlap.y = overlap;
		axis = Vec3(0, -1, 0);
	}
	overlap = std::abs(b1->max.y - min.y);
	if (overlap < minOverlap.y) {
		minOverlap.y = overlap;
		axis = Vec3(0, 1, 0);
	}
	overlap = std::abs(b1->min.z - max.z);
	if (overlap < minOverlap.z) {
		minOverlap.z = overlap;
		axis = Vec3(0, 0, -1);
	}
	overlap = std::abs(b1->max.z - min.z);
	if (overlap < minOverlap.z) {
		minOverlap.z = overlap;
		axis = Vec3(0, 0, 1);
	}
	
	return axis * VMath::dot(axis, minOverlap);
}

bool PHYSICS::AABB::TestAABB(Ref<AABB> b1, Ref<AABB> b2)
{
	if (b1->min.x >= b2->max.x) return false;	
	if (b1->max.x <= b2->min.x) return false;

	if (b1->min.y >= b2->max.y) return false;
	if (b1->max.y <= b2->min.y) return false;

	if (b1->min.z >= b2->max.z) return false;
	if (b1->max.z <= b2->min.z) return false;
		
	//printf("Collision!\n");	
	
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
		SetCentre(ParentTransform->GetPosition() + Vec3(0.0f, 1.7f, 0.0f));
	}
	SetBounds(scale, centre, orientation);
	Add(bounds);
}
