#pragma once


#include "Component.h"
#include "Actor.h"
#include "RoomScene.h"

namespace PHYSICS
{
	class DynamicLinearMovement : public Component
	{
		const Vec3 Gravity = Vec3(0.0f, -9.81f, 0.0f);
		Ref<TransformComponent> TransformRef;
		const float FallSpeed = 1600.0f;
		Vec3 vel;
		Vec3 accel;
		Vec3 pos;
		Quaternion orient;
		const float mass = 1.0f;
		bool collision = false;
		//Ref<RoomScene> room;

		void UpdateFall(const float deltaTime);
		void UpdateWall(const float deltaTime);

	public:

		DynamicLinearMovement();
		DynamicLinearMovement(Component* parent_, const Ref<TransformComponent>& playerTrans);		

		const bool GetCollision() const { return collision; }
		void SetCollision(bool value) { collision = value; }

		const Vec3 GetAccel() const { return accel; }
		const Vec3 GetVel() const { return vel; }


		void SetVel(Vec3 vel_) { vel = vel_; }

		virtual ~DynamicLinearMovement();
		virtual bool OnCreate() { return true; }
		virtual void OnDestroy();
		virtual void Update(const float deltaTime_);
		virtual void Render()const {}		
	};

}