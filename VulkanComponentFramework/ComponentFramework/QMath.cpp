#include <algorithm>    // std::min, std::clamp
#include "QMath.h"
#include "Matrix.h"
#include "Euler.h"
using namespace MATH;

float QMath::magnitude(const Quaternion& q) {
	return(sqrt((q.w * q.w) + (q.ijk.x * q.ijk.x) + (q.ijk.y * q.ijk.y) + (q.ijk.z * q.ijk.z)));
}

Quaternion QMath::conjugate(const Quaternion& q) {
	return Quaternion(q.w, -q.ijk);
}

Quaternion QMath::inverse(const Quaternion& q) {
	float mag = magnitude(q);
	Quaternion conj = conjugate(q);
	return  conj / (mag * mag);
}

Quaternion QMath::pow(const Quaternion& q, float exponent) {
	/// Watch out, if q.w is +/- 1, acos(q.w) will be 0 and cause divide by zero exception
	if (fabs(q.w) > VERY_CLOSE_TO_ONE) return q;

	float alpha = acos(q.w);
	float newAlpha = alpha * exponent;
	return Quaternion(cos(newAlpha), q.ijk * sin(newAlpha) / sin(alpha));
}


Quaternion QMath::normalize(const Quaternion& q) {
	return q / magnitude(q);
}

Euler QMath::fromQuaternion(const Quaternion& q) {
	Euler result;
	result.roll = atan2(2.0f * (q.ijk.x * q.ijk.y + q.w * q.ijk.z), q.w * q.w + q.ijk.x * q.ijk.x - q.ijk.y * q.ijk.y - q.ijk.z * q.ijk.z);
	result.pitch = atan2(2.0f * (q.ijk.y * q.ijk.z + q.w * q.ijk.x), q.w * q.w - q.ijk.x * q.ijk.x - q.ijk.y * q.ijk.y + q.ijk.z * q.ijk.z);
	result.yaw = asin(std::clamp(-2.0f * (q.ijk.x * q.ijk.z - q.w * q.ijk.y), -1.0f, 1.0f));
	return result * RADIANS_TO_DEGREES;
}

Quaternion QMath::fromEuler(const Euler& e) {
	float cosX = cos(0.5f * e.xAxis * DEGREES_TO_RADIANS);
	float cosY = cos(0.5f * e.yAxis * DEGREES_TO_RADIANS);
	float cosZ = cos(0.5f * e.zAxis * DEGREES_TO_RADIANS);
	float sinX = sin(0.5f * e.xAxis * DEGREES_TO_RADIANS);
	float sinY = sin(0.5f * e.yAxis * DEGREES_TO_RADIANS);
	float sinZ = sin(0.5f * e.zAxis * DEGREES_TO_RADIANS);

	/// For XYZ order
	return Quaternion((cosX * cosY * cosZ) + (sinX * sinY * sinZ),
		Vec3((sinX * cosY * cosZ) - (cosX * sinY * sinZ),
			(cosX * sinY * cosZ) + (sinX * cosY * sinZ),
			(cosX * cosY * sinZ) - (sinX * sinY * cosZ)));
}

Matrix3 QMath::toMatrix3(const Quaternion& q) {
	/// This is the fastest way I know...
	return Matrix3((1.0f - 2.0f * q.ijk.y * q.ijk.y - 2.0f * q.ijk.z * q.ijk.z), (2.0f * q.ijk.x * q.ijk.y + 2.0f * q.ijk.z * q.w), (2.0f * q.ijk.x * q.ijk.z - 2.0f * q.ijk.y * q.w),
		(2.0f * q.ijk.x * q.ijk.y - 2.0f * q.ijk.z * q.w), (1.0f - 2.0f * q.ijk.x * q.ijk.x - 2.0f * q.ijk.z * q.ijk.z), (2.0f * q.ijk.y * q.ijk.z + 2.0f * q.ijk.x * q.w),
		(2.0f * q.ijk.x * q.ijk.z + 2.0f * q.ijk.y * q.w), (2.0f * q.ijk.y * q.ijk.z - 2 * q.ijk.x * q.w), (1.0f - 2.0f * q.ijk.x * q.ijk.x - 2.0f * q.ijk.y * q.ijk.y));
}

Matrix4 QMath::toMatrix4(const Quaternion& q) {
	/// This is the fastest way I know...
	return Matrix4((1.0f - 2.0f * q.ijk.y * q.ijk.y - 2.0f * q.ijk.z * q.ijk.z), (2.0f * q.ijk.x * q.ijk.y + 2.0f * q.ijk.z * q.w), (2.0f * q.ijk.x * q.ijk.z - 2.0f * q.ijk.y * q.w), 0.0f,
		(2.0f * q.ijk.x * q.ijk.y - 2.0f * q.ijk.z * q.w), (1.0f - 2.0f * q.ijk.x * q.ijk.x - 2.0f * q.ijk.z * q.ijk.z), (2.0f * q.ijk.y * q.ijk.z + 2.0f * q.ijk.x * q.w), 0.0f,
		(2.0f * q.ijk.x * q.ijk.z + 2.0f * q.ijk.y * q.w), (2.0f * q.ijk.y * q.ijk.z - 2 * q.ijk.x * q.w), (1.0f - 2.0f * q.ijk.x * q.ijk.x - 2.0f * q.ijk.y * q.ijk.y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	/// ... but this is the coolest. My way is just a bit faster on single processor machines,
	/// this method is faster on parallel multicore machines. Multicore can calc m1 and m2
	/// on separate threads. Just saying. 

	//Matrix4 m1( q.w,  q.ijk.z,  -q.ijk.y,  q.ijk.x,
	//		-q.ijk.z,   q.w,   q.ijk.x,  q.ijk.y,
	//		q.ijk.y,  -q.ijk.x,   q.w,  q.ijk.z,
	//		-q.ijk.x,  -q.ijk.y,  -q.ijk.z,  q.w);
	//
	//Matrix4 m2( q.w,   q.ijk.z,  -q.ijk.y,  -q.ijk.x,
	//			-q.ijk.z,   q.w,   q.ijk.x,  -q.ijk.y,
	//			q.ijk.y,  -q.ijk.x,   q.w,  -q.ijk.z,
	//			-q.ijk.x,   q.ijk.y,   q.ijk.z,   q.w);
	//return m1 * m2;

}

Quaternion MATH::QMath::mat3ToQuaternion(const float* m)
{
	float trace = m[0] + m[4] + m[8];
	float w, x, y, z;

	if (trace > 0) {
		float s = sqrt(trace + 1.0f) * 2.0f;
		w = 0.25f * s;
		x = (m[7] - m[5]) / s;
		y = (m[2] - m[6]) / s;
		z = (m[3] - m[1]) / s;
	}
	else if ((m[0] > m[4]) && (m[0] > m[8])) {
		float s = sqrt(1.0f + m[0] - m[4] - m[8]) * 2.0f;
		w = (m[7] - m[5]) / s;
		x = 0.25f * s;
		y = (m[1] + m[3]) / s;
		z = (m[2] + m[6]) / s;
	}
	else if (m[4] > m[8]) {
		float s = sqrt(1.0f + m[4] - m[0] - m[8]) * 2.0f;
		w = (m[2] - m[6]) / s;
		x = (m[1] + m[3]) / s;
		y = 0.25f * s;
		z = (m[5] + m[7]) / s;
	}
	else {
		float s = sqrt(1.0f + m[8] - m[0] - m[4]) * 2.0f;
		w = (m[3] - m[1]) / s;
		x = (m[2] + m[6]) / s;
		y = (m[5] + m[7]) / s;
		z = 0.25f * s;
	}

	// Normalize quaternion	
	// Return quaternion
	return normalize(Quaternion(w, x, y, z));
}

Quaternion MATH::QMath::mat4ToQuaternion(const float* m)
{
	float trace = m[0] + m[5] + m[10];
	float w, x, y, z;

	if (trace > 0) {
		float s = sqrt(trace + 1.0f) * 2.0f;
		w = 0.25f * s;
		x = (m[9] - m[6]) / s;
		y = (m[2] - m[8]) / s;
		z = (m[4] - m[1]) / s;
	}
	else if ((m[0] > m[5]) && (m[0] > m[10])) {
		float s = sqrt(1.0f + m[0] - m[5] - m[10]) * 2.0f;
		w = (m[9] - m[6]) / s;
		x = 0.25f * s;
		y = (m[1] + m[4]) / s;
		z = (m[2] + m[8]) / s;
	}
	else if (m[5] > m[10]) {
		float s = sqrt(1.0f + m[5] - m[0] - m[10]) * 2.0f;
		w = (m[2] - m[8]) / s;
		x = (m[1] + m[4]) / s;
		y = 0.25f * s;
		z = (m[6] + m[9]) / s;
	}
	else {
		float s = sqrt(1.0f + m[10] - m[0] - m[5]) * 2.0f;
		w = (m[4] - m[1]) / s;
		x = (m[2] + m[8]) / s;
		y = (m[6] + m[9]) / s;
		z = 0.25f * s;
	}	

	// Normalize quaternion	
	// Return quaternion
	return normalize(Quaternion(w, x, y, z));
}
Quaternion QMath::angleAxisRotation(const float degrees, const Vec3& axis) {
	Vec3 rotationAxis = VMath::normalize(axis);
	float theta = degrees * DEGREES_TO_RADIANS;
	float cosVal = cos(theta / 2.0f);
	float sinVal = sin(theta / 2.0f);
	return Quaternion(cosVal, rotationAxis * sinVal);
}


Vec3 QMath::rotate(const Vec3& v, const Quaternion& q) {
	/// This is the beauty of Quaternions 
	Quaternion p(0.0, v); /// convert the incoming vector to a Quaternion
	Quaternion qInv = QMath::inverse(q); /// Get the inverse - duh.
	Quaternion result = q * p * qInv;
	return Vec3(result.ijk);

	/// I got the idea to do it this way from glm. Its faster says glm. 
	/***Vec3 quatVector(q.v);
	Vec3 uv = VMath::cross(q.v, v);
	Vec3 uuv = VMath::cross(q.v, uv);
	return v + ((uv * q.w) + uuv) * 2.0f;***/


}