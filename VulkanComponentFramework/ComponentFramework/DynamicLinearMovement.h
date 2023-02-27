#pragma once


#include "Component.h"
#include "TransformComponent.h"


namespace PHYSICS
{
	class DynamicLinearMovement : public Component
	{
		const float Gravity = 9.81f;
		Ref<TransformComponent> MovementTransform;
		const float FallSpeed = 1600.0f;



	public:

		DynamicLinearMovement();
		DynamicLinearMovement(Component* parent_, Ref<TransformComponent> TransformComponent_);		

		virtual ~DynamicLinearMovement();
		virtual bool OnCreate() { return true; }
		virtual void OnDestroy();
		virtual void Update(const float deltaTime_);
		virtual void Render()const {}

	};

}