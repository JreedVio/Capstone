#include "Physics.h"

void PHYSICS::ApplyTorque(Ref<PhysicsComponent> body, MATH::Vec3 torque)
{
	body->angularAcc = MATH::MMath::inverse(body->rotationalInertia) * torque;
}

void PHYSICS::ApplyForce(Ref<PhysicsComponent> body, MATH::Vec3 torque)
{
	body->accel = torque / body->mass;
}

void PHYSICS::ApplyAngularMotion(Ref<PhysicsComponent> body, const float deltaTime)
{
	body->angularVel += body->angularAcc * deltaTime;
	// update orientation too
	MATH::Quaternion angularVelQuaternion(0.0, body->angularVel);
	// Rotate using qq +0.5twq
	body->orientation = body->orientation + angularVelQuaternion * body->orientation * 0.5f * deltaTime;
	// Normalized so body doesnt stretch
	body->orientation = MATH::QMath::normalize(body->orientation);
}

void PHYSICS::ApplyLinearMotion(Ref<PhysicsComponent> body, const float deltaTime)
{
	UpdatePos(body, deltaTime);
	UpdateVel(body, deltaTime);
}

void PHYSICS::UpdatePos(Ref<PhysicsComponent> body, const float deltaTime)
{
	body->pos += body->vel * deltaTime + 0.5f * body->accel * deltaTime * deltaTime;
}

void PHYSICS::UpdateVel(Ref<PhysicsComponent> body, const float deltaTime)
{
	body->vel += body->accel * deltaTime;
}

void PHYSICS::UpdateTransform(const Ref<PhysicsComponent> body, Ref<TransformComponent> transform)
{
	transform->pos = body->pos;
	transform->orientation = body->orientation;
}
