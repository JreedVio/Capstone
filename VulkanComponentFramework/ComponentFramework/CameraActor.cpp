#include <glew.h>
#include "CameraActor.h"
#include "MMath.h"
#include "QMath.h"
#include "TransformComponent.h"
#include "Debug.h"
using namespace MATH;
CameraActor::CameraActor(Component* parent_): Actor(parent_) {
	
}

CameraActor::~CameraActor() {
	OnDestroy();
}

bool CameraActor::OnCreate() {
	if (isCreated) return isCreated;
	Debug::Info("Creating values for CameraActor: ", __FILE__, __LINE__);

	UpdateProjectionMatrix(45.0f, (16.0f / 9.0f), 0.5f, 100.0f); /// default projection
	UpdateViewMatrix();

	isCreated = true;
	return isCreated;
}

void CameraActor::UpdateViewMatrix() {
	Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>();
	if (transformComponent == nullptr) { /// There is no such component, use default view
		viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		Quaternion orientation = transformComponent->GetOrientation();
		Vec3 position = transformComponent->GetPosition();
		viewMatrix = MMath::translate(position) * QMath::toMatrix4(orientation);
	}
}

void CameraActor::UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far) {

}

void CameraActor::OnDestroy() {
	isCreated = false;
}