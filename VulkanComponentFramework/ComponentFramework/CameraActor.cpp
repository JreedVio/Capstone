#include <glew.h>
#include <SDL.h>
#include "CameraActor.h"
#include "MMath.h"
#include "QMath.h"
#include "TransformComponent.h"
#include "Debug.h"
#include "VulkanRenderer.h"
//#include "UBO_Padding.h"
using namespace MATH;

CameraActor::CameraActor(Component* parent_): Actor(parent_) {
	projectionMatrix.loadIdentity();
	viewMatrix.loadIdentity();
	forwardVec = Vec3(0.0f, 0.0f, -1.0f);
	minZ = 1.0f;
	maxZ = 4.0f;
	cameraSpeed = 1.0f;
}

CameraActor::~CameraActor() {
	OnDestroy();
}

bool CameraActor::OnCreate() {
	if (isCreated) 	return isCreated; //or return true

	
	//viewMatrix = MMath::rotate(0.0f, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(0.0f, 0.0f, -5.0f);
	UpdateProjectionMatrix(45.0f, (16.0f / 9.0f), 0.5f, 100.0f); /// default projection
	UpdateViewMatrix();

	isCreated = true;
	return isCreated;
}

void CameraActor::OnDestroy(){
	isCreated = false;
}

void CameraActor::Render() const {

}

void CameraActor::Update(const float deltaTime){
	//UpdateViewMatrix();
}

void CameraActor::HandleEvents(const SDL_Event & sdlEvent){

	Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();

	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			//std::cout << "LEFT EVENT\n";
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.1f, 0.0f, 0.0f), transform_->GetOrientation());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			//std::cout << "RIGHT EVENT\n";
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(-0.1f, 0.0f, 0.0f), transform_->GetOrientation());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			//std::cout << "UP EVENT\n";
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, 0.1f), transform_->GetOrientation());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			//std::cout << "DOWN EVENT\n";
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, -0.1f), transform_->GetOrientation());
			UpdateViewMatrix();
		}
		break;

	case SDL_MOUSEWHEEL:
		if (sdlEvent.wheel.y > 0) {// scroll up
			Vec3 newPos = transform_->GetPosition() + Vec3(0.0f, 0.0f, 0.1f);
			if (newPos.z >= maxZ) {
				newPos.z = maxZ;
			}
			transform_->SetTransform(
				newPos, transform_->GetOrientation());
			UpdateViewMatrix();
		}
		else if (sdlEvent.wheel.y < 0) { // scroll down
			Vec3 newPos = transform_->GetPosition() + Vec3(0.0f, 0.0f, -0.1f);
			if (newPos.z <= minZ) {
				newPos.z = minZ;
			}
			transform_->SetTransform(
				newPos, transform_->GetOrientation());
			UpdateViewMatrix();
		}
		break;

	case SDL_MOUSEMOTION:
		//TODO:Smoother movement (Optional)

		/*Find current forward vector of the camera
		* The old quaternion will be take into account at the end, 
		* so this vector does not need to be calculated through current orientation 
		*/
		Vec3 rotationAxis;

		//Find the destination vector that the current forward vector should rotate to
		Vec3 mousePos = Vec3(sdlEvent.motion.x, sdlEvent.motion.y, 0.0f);
		Vec3 delta = (mousePos - lastMousePos);
		float smoothValue = VERY_SMALL + 10.0f;
		//Limit the camera upgrade interval
		if (VMath::mag(delta) <= smoothValue && lastMousePos != Vec3(0.0f, 0.0f, 0.0f)) {
			break;
		}
		Vec3 current = forwardVec;
		Vec3 cameraPos = transform_->GetPosition();
		if (parent) {
			Ref<TransformComponent> parentTransform = dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>();
			cameraPos = cameraPos - parentTransform->GetPosition();
		}
		//Transform from screen coords to world coords
		VulkanRenderer* renderer_ = VulkanRenderer::GetInstance();
		Matrix4 ndc = MMath::viewportNDC(renderer_->GetWidth(), renderer_->GetHeight());
		Matrix4 projection = GetProjectionMatrix();
		Matrix4 view = GetViewMatrix();
		Matrix4 worldTransform = MMath::inverse(ndc * projection * view);
		Vec3 worldMousePos = worldTransform * mousePos;
		Vec3 destination = worldMousePos - cameraPos;
		/*Use the dot products and the magnitude to find the cosine
		* This cosine is usually around 1.0f due to the normalized world coordinates
		* (Really no point to calculate the angle, just do it anyways)
		*/
		float magDesition = VMath::mag(destination);
		float magAxis = VMath::mag(current);
		float cosine = abs(VMath::dot(current, destination) / (magAxis * magDesition));

		//Calculate the rotational axis
		rotationAxis = VMath::cross(VMath::normalize(destination), current);
		//Use the calculated angle and rotation axis to get the rotation
		//Add the camera velocity as a factor to the angle,
		Quaternion newOreintation = QMath::angleAxisRotation(cosine * cameraSpeed, rotationAxis) * transform_->GetOrientation();
		Vec3 ijk = newOreintation.ijk;
		//Make the rotation around z axis fixed
		newOreintation.set(newOreintation.w, ijk.x, ijk.y, 0.0f);
		transform_->SetTransform(transform_->GetPosition(), newOreintation);
		UpdateViewMatrix();

		//Record last mouse position and forward vector
		lastMousePos = mousePos;
		forwardVec = VMath::normalize(destination);
		//forwardVec = QMath::rotate(Vec3(0.0f, 0.0f, -1.0f), transform_->GetOrientation());

		break;
	}
}

void CameraActor::UpdateViewMatrix() {
	Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>();

	
	if (transformComponent.get() == nullptr) { /// There is no such component, use default view
		transformComponent = std::make_shared<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -5.0f), Quaternion());
		AddComponent(transformComponent);
		//viewMatrix = MMath::translate(0.0f, 0.0f, -5.0f) * MMath::rotate(0.0f, Vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	}
	else {
		Quaternion orientation = transformComponent->GetOrientation();
		Vec3 position = transformComponent->GetPosition();
		if (parent) {
			Ref<TransformComponent> parentTransform = dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>();
			position = position - parentTransform->GetPosition() ;
		}
		viewMatrix = QMath::toMatrix4(orientation) * MMath::translate(position);

	}
	//Quaternion orientation = transformComponent->GetQuaternion();
	//Vec3 position = transformComponent->GetPosition();
	//viewMatrix = QMath::toMatrix4(orientation) * MMath::translate(position);


}

void CameraActor::UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far) {
	projectionMatrix = MMath::perspective(fovy, aspectRatio, near, far);

}