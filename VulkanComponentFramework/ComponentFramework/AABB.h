#pragma once
#include "Component.h"
#include "VMath.h"
#include "QMath.h"

using namespace MATH;

namespace PHYSICS
{
	class AABB : public Component
	{

		Vec3 bounds[8];
		Vec3 centre;
		Vec3 min;
		Vec3 max;
		Vec3 scale;
		Quaternion orientation;

	public:


		AABB(Component* parent_, Vec3 centre_ = Vec3(0.0f, 0.0f, 0.0f), Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f),
			Quaternion orientation_ = Quaternion(1.0f, 0.0f, 0.0f, 0.0f));

		AABB(Vec3 centre_ = Vec3(0.0f, 0.0f, 0.0f), Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f),
			 Quaternion orientation_ = Quaternion(1.0f, 0.0f, 0.0f, 0.0f));


		void Empty()
		{
			min.x = min.y = min.z = FLT_MAX;
			max.x = max.y = max.z = -FLT_MAX;
		}

		void SetBounds(const Vec3 scale_, const Vec3 centre_, const Quaternion orient_);

		void Add(const Vec3 bounds_[]);

		static bool TestAABB(AABB* b1, AABB* b2);

		void SetCentre(const Vec3 centre_) { centre = centre_; }

		virtual ~AABB();
		virtual bool OnCreate() { return true; }
		virtual void OnDestroy();
		virtual void Update(const float deltaTime_);
		virtual void Render()const {}


		// sample code for defining a box with points
		/*
			const int N;
			Vec3 Points[N];

			AABB box;
			box.Empty();

			for (int i = 0; i < N; ++i)
			{
				box.Add(Points[i]);
			}
		*/

		/* !!TESTING ONLY !!DELETE AFTER !!; begin
		{
			const int N = 8;
			Vec3 Points1[N];
			Vec3 Points2[N];

			Points1[0] = Vec3(0, 0, 0);
			Points1[1] = Vec3(2, 0, 0);
			Points1[2] = Vec3(0, 2, 0);
			Points1[3] = Vec3(2, 2, 0);

			Points1[4] = Vec3(0, 0, 2);
			Points1[5] = Vec3(2, 0, 2);
			Points1[6] = Vec3(0, 2, 2);
			Points1[7] = Vec3(2, 2, 2);

			Points2[0] = Vec3(0, 0, 13);
			Points2[1] = Vec3(1.5f, 0, 13);
			Points2[2] = Vec3(0, 1.5f, 13);
			Points2[3] = Vec3(1.5f, 1.5f, 13);

			Points2[4] = Vec3(0, 0, 15);
			Points2[5] = Vec3(1.5f, 0, 15);
			Points2[6] = Vec3(0, 1.5f, 15);
			Points2[7] = Vec3(1.5f, 1.5f, 15);

			AABB box1;
			AABB box2;
			box1.Empty();
			box2.Empty();

			for (int i = 0; i < N; ++i)
			{
				box1.Add(Points1[i]);
				box2.Add(Points2[i]);
			}

			if (Physics::TestTwoAABB(box1, box2))
			{
				printf("Intersection!\n");
			}
			else
			{
				printf("No Intersections!\n");
			}
		}
		*/// !! TESTING ONLY !! DELETE AFTER !!; end

	};

}