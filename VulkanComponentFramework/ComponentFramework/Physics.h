#pragma once


// This class will hold structures or classes that contain geometric data

#include "VMath.h"
#include "MMath.h"
#include "QMath.h"

using namespace MATH;

namespace PHYSICS
{

	class AABB
	{

		Vec3 bounds[8];
		Vec3 centre;
		Vec3 min;
		Vec3 max;
		Vec3 scale;
		Quaternion orientation;

	public:


		AABB(Vec3 centre_ = Vec3(0.0f, 0.0f, 0.0f), Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f),
			Quaternion orientation_ = Quaternion(0.0f, 1.0f, 0.0f, 0.0f))
		{
			Empty();

			scale = scale_;
			centre = centre_;
			orientation = orientation_;

			SetBounds(scale, centre, orientation);
			Add(bounds);
		}

		void Empty()
		{
			min.x = min.y = min.z = FLT_MAX;
			max.x = max.y = max.z = -FLT_MAX;
		}

		void SetBounds(const Vec3& scale_, const Vec3& centre_, const Quaternion& orient_)
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

		void Add(const Vec3 bounds_[])
		{
			for (int i = 0; i < 8; ++i)
			{
				if (bounds_[i].x < min.x) min.x = bounds_[i].x;
				if (bounds_[i].x > max.x) max.x = bounds_[i].x;

				if (bounds_[i].y < min.y) min.y = bounds_[i].y;
				if (bounds_[i].y > max.y) max.y = bounds_[i].y;

				if (bounds_[i].z < min.z) min.z = bounds_[i].z;
				if (bounds_[i].z > max.z) max.z = bounds_[i].z;
			}
		}

		static bool TestAABB(const AABB& b1, const AABB& b2)
		{
			if (b1.min.x >= b2.max.x) return false;
			if (b1.max.x <= b2.min.x) return false;

			if (b1.min.y >= b2.max.y) return false;
			if (b1.max.y <= b2.min.y) return false;

			if (b1.min.z >= b2.max.z) return false;
			if (b1.max.z <= b2.min.z) return false;

			return true;
		}

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


	class Physics
	{




	public:
				
		static bool TestTwoAABB(const AABB& b1, const AABB& b2);
		
	};

}