#pragma once
#include "Actor.h"
#include "Matrix.h"
using namespace MATH;

union SDL_Event;

class CameraActor:public Actor {
private:
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Vec3 lastMousePos;
	Vec3 forwardVec;
	float cameraSpeed;
	float minZ;
	float maxZ;

public:
	CameraActor(Component* parent_);
	virtual ~CameraActor();

	bool OnCreate() override;
	void OnDestroy() override;
	void Render() const;
	void Update(const float deltaTime) override;
	void HandleEvents(const SDL_Event& sdlEvent);
	Matrix4 GetProjectionMatrix() const { return projectionMatrix; }
	Matrix4 GetViewMatrix() const { return viewMatrix; }
	Vec3 GetForward() { return forwardVec; }

	void UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far);
	void UpdateViewMatrix();
	PushConst GetModelMatrix() override;
	Quaternion GetRotation();
};

