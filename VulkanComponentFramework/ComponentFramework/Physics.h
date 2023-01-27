#ifndef PHYSICS_H
#define PHYSICS_H
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "MMath.h"
#include "QMath.h"

namespace PHYSICS {
	//Updates angular accelration using torque/rotational inertia
	void ApplyTorque(Ref<PhysicsComponent> body, MATH::Vec3 torque);

	//Updates acceleration using a = F/m
	void ApplyForce(Ref<PhysicsComponent> body, MATH::Vec3 torque);

	//Updates orientation and angular velocity using rotational equations of motion
	void ApplyAngularMotion(Ref<PhysicsComponent> body, const float deltaTime);

	//Updates posiion and linear velocity using linear equations of motion
	void ApplyLinearMotion(Ref<PhysicsComponent> body, const float deltaTime);

	//Update position or velocity
	void UpdatePos(Ref<PhysicsComponent> body, const float deltaTime);
	void UpdateVel(Ref<PhysicsComponent> body, const float deltaTime);

	//Update body transform
	void UpdateTransform(const Ref<PhysicsComponent> body, Ref<TransformComponent> transform);
}
#endif
