#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Component.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

struct PhysicsComponent : public Component {
	float mass = 1.0f;
	MATH::Matrix3 rotationalInertia;
	MATH::Vec3 pos;
	MATH::Vec3 vel;
	MATH::Vec3 accel;
	MATH::Quaternion orientation;
	MATH::Vec3 angularVel;
	MATH::Vec3 angularAcc;

	PhysicsComponent(Component* parent_) : Component(parent_) {};
	PhysicsComponent(Component* parent_, float mass_, MATH::Matrix3 rotationalInertia_, MATH::Vec3 pos_, MATH::Vec3 vel_,
		MATH::Vec3 accel_, MATH::Quaternion orientation_, MATH::Vec3 angularVel_, MATH::Vec3 angularAcc_)
		: Component(parent_), mass(mass_), rotationalInertia(rotationalInertia_), pos(pos_), vel(vel_), accel(accel_),
		orientation(orientation_), angularVel(angularVel_), angularAcc(angularAcc_) {};
	~PhysicsComponent() {};

	bool OnCreate() {
		return true;
	};
	void OnDestroy() {};
	void Update(const float deltaTime_) {};
	void Render() const {};
};
#endif