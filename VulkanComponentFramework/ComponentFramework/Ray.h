#ifndef RAY_H
#define RAY_H
#include "Vector.h"
#include "VMath.h"

namespace GEOMETRY {
	struct RayIntersectInfo {
		bool isIntersected = false;
		MATH::Vec3 intersectionPoint;
		float t = 0.0f;

		void print() {
			if (isIntersected) {
				std::cout << "Ray intersection with t value: " << t << " at position: ";
				intersectionPoint.print();
			}
			else {
				std::cout << "No ray intersection.\n";
			}
		}
	};
	struct Ray {
		MATH::Vec3 dir;
		MATH::Vec3 start;
		Ray() {
			dir.set(0.0f, 0.0f, -1.0f);
		}
		Ray(MATH::Vec3& start_, MATH::Vec3& dir_) {
			set(start_, dir_);
		}

		void set(const MATH::Vec3& start_, const MATH::Vec3& dir_) {
			start = start_;
			dir = dir_;
		}

		//P(t)P + V * T
		MATH::Vec3 currentPostion(float t) const { return start + t * dir; }
	};
};
#endif
