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
	pos.y += FallSpeed * deltaTime * 0.5f * -Gravity * deltaTime * deltaTime;

	MovementTransform->SetTransform(pos, orient);
}

