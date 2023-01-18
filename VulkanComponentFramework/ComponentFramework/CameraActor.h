#pragma once
#include "Actor.h"
#include "Matrix.h"
using namespace MATH;

class CameraActor:public Actor {
private:
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	
public:
	CameraActor(Component* parent_);
	virtual ~CameraActor();

	bool OnCreate() override;
	void OnDestroy() override;
	Matrix4 GetProjectionMatrix() const { return projectionMatrix; }
	Matrix4 GetViewMatrix() const { return viewMatrix; }

	void UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far);
	void UpdateViewMatrix();
};

