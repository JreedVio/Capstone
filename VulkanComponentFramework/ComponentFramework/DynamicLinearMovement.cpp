#include "DynamicLinearMovement.h"

PHYSICS::DynamicLinearMovement::DynamicLinearMovement()
{
}

PHYSICS::DynamicLinearMovement::DynamicLinearMovement(Component* parent_, Ref<TransformComponent> TransformComponent_) : Component(parent_),
											 MovementTransform(TransformComponent_)
{
}

PHYSICS::DynamicLinearMovement::~DynamicLinearMovement()
{
}

void PHYSICS::DynamicLinearMovement::OnDestroy()
{
}

void PHYSICS::DynamicLinearMovement::Update(const float deltaTime)
{
	if (!MovementTransform)
	{
		printf("MovementTransform is nullptr!\n");
		return;
	}

	Quaternion orient = MovementTransform->GetOrientation();
	Vec3 pos = MovementTransform->GetPosition();
	accel = Gravity / mass;
	pos.y += vel.y * deltaTime + accel.y * (0.5f * deltaTime * deltaTime);
	vel += accel * deltaTime;

	MovementTransform->SetTransform(pos, orient);
}

void PHYSICS::DynamicLinearMovement::ApplyForce(Vec3 force_){
	accel = force_ / mass;
}

