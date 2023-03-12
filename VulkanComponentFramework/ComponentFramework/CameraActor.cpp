#include <glew.h>
#include <SDL.h>
#include "CameraActor.h"
#include "MMath.h"
#include "QMath.h"
#include "TransformComponent.h"
#include "Debug.h"
#include "VulkanRenderer.h"

using namespace MATH;

CameraActor::CameraActor(Component* parent_): Actor(parent_), isActivate(false) {
	projectionMatrix.loadIdentity();
	viewMatrix.loadIdentity();
	forwardVec = Vec3(0.0f, 0.0f, -1.0f);
	minZ = 1.0f;
	maxZ = 4.0f;
	cameraSpeed = 0.3f;
	lastMousePos = Vec3(0.0f, 0.0f, 0.0f);
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
	UpdateViewMatrix();
}

void CameraActor::HandleEvents(const SDL_Event & sdlEvent){
	VulkanRenderer* renderer_ = VulkanRenderer::GetInstance();
	float w_ = renderer_->GetWidth();
	float h_ = renderer_->GetHeight();
	Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
	//Transform from screen coords to world coords
	Matrix4 ndc = MMath::viewportNDC(w_, h_);
	Matrix4 projection = GetProjectionMatrix();
	Matrix4 view = GetViewMatrix();
	Matrix4 worldTransform = MMath::inverse(ndc * projection * view);

	Vec3 forward = Vec3(0.0f, 0.0f, 1.0f);
	forwardVec = QMath::rotate(forward, transform_->GetOrientation());
	forwardVec.y = 0.0f;

	Vec3 upVector = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 rightDirection = VMath::cross(forwardVec, upVector);

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

	case SDL_MOUSEBUTTONDOWN: 
		isActivate = true;
		lastMousePos.x = sdlEvent.button.x;
		lastMousePos.y = sdlEvent.button.y;

		break;
	
	case SDL_MOUSEMOTION:

		if (!isActivate) return;

		Vec3 rotationAxis;
		//Find the destination vector that the current forward vector should rotate to
		Vec3 mousePos = Vec3(sdlEvent.motion.x, sdlEvent.motion.y, 0.0f);
		Vec3 delta = (mousePos - lastMousePos);
		float smoothValue = VERY_SMALL;
		//Limit the camera upgrade interval
		//if (VMath::mag(delta) <= smoothValue) {
		//	break;
		//}
		Vec3 current = forwardVec;
		Vec3 cameraPos = transform_->GetPosition();
		if (parent) {
			Ref<TransformComponent> parentTransform = dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>();
			cameraPos = cameraPos - parentTransform->GetPosition();
		}

		/*Code not used*/
		//Vec3 worldMousePos = worldTransform * mousePos;
		//Vec3 destination = worldMousePos - cameraPos;

		///*Use the dot products and the magnitude to find the cosine
		//* This cosine is usually around 1.0f due to the normalized world coordinates
		//* (Really no point to calculate the angle, just do it anyways)
		//*/
		//float magDesition = VMath::mag(destination);
		//float magAxis = VMath::mag(current);
		//float cosine = abs(VMath::dot(current, destination) / (magAxis * magDesition));
		//Calculate the rotational axis
		//rotationAxis = VMath::cross(VMath::normalize(destination), VMath::normalize(current));
		//Use the calculated angle and rotation axis to get the rotation
		//Add the camera velocity as a factor to the angle,
		//Quaternion newOreintation = QMath::angleAxisRotation(cosine * cameraSpeed, rotationAxis) * transform_->GetOrientation();
		//Vec3 ijk = newOreintation.ijk;
		//Make the rotation around z axis fixed
		//newOreintation.set(newOreintation.w, ijk.x, ijk.y, 0.0f);

		//Do separate rotation in x axis and y axis
		//x axis rotaiton
		if (abs(delta.x) >= VERY_SMALL) {
			if (abs(delta.x) >= w_ / 2.0f) delta.x /= w_;
			Quaternion newOrientation_ = QMath::angleAxisRotation(delta.x * cameraSpeed, upVector) * transform_->GetOrientation();
			transform_->SetTransform(transform_->GetPosition(), newOrientation_);
			//Rotate the actor attached if there is one
			if (parent) {
				Ref<TransformComponent> parentTransform = dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>();
				parentTransform->SetTransform(parentTransform->GetPosition(),
									QMath::angleAxisRotation(-delta.x * cameraSpeed, upVector) * parentTransform->GetOrientation());
			}
		}

		//Find forward vector and right direction vector
		forwardVec = QMath::rotate(forward, transform_->GetOrientation());
		forwardVec.y = 0.0f;
		rightDirection = VMath::cross(forwardVec, upVector);

		//y axis rotation
		if (abs(delta.y) >= VERY_SMALL) {
			if (abs(delta.y) >= w_ / 2.0f) delta.y /= w_;

			Quaternion newOrientation_ = QMath::angleAxisRotation(-delta.y * cameraSpeed, rightDirection) * transform_->GetOrientation();
			Vec3 ijk = newOrientation_.ijk;
			//Limit the y axis rotation
			//Predict the vector after rotation, and check the y axis
			Vec3 destination = QMath::rotate(forward, newOrientation_);
			destination *= -1.0f; //flip upside down
			//if (ijk.x >= 0.0f && ijk.x <= 0.05f) {
			if (destination.y >= -0.2f && destination.y <= 0.35f){
				transform_->SetTransform(transform_->GetPosition(), newOrientation_);
			}
		}

		//Record last mouse position
		lastMousePos = mousePos;
		UpdateViewMatrix();
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
			Actor* parentActor = dynamic_cast<Actor*>(parent);
			Ref<TransformComponent> parentTransform = parentActor->GetComponent<TransformComponent>();
			position = position - parentTransform->GetPosition();
			orientation = transformComponent->GetOrientation();
		}
		viewMatrix = QMath::toMatrix4(orientation) * MMath::translate(position);
	}
	//Quaternion orientation = transformComponent->GetQuaternion();
	//Vec3 position = transformComponent->GetPosition();
	//viewMatrix = QMath::toMatrix4(orientation) * MMath::translate(position);

}

PushConst CameraActor::GetModelMatrix(){
	Ref<TransformComponent> transform = GetComponent<TransformComponent>();
	if (transform) {
		Vec3 pos = transform->GetPosition();
		Quaternion orientation_ = transform->GetOrientation();
		Vec3 ijk = orientation_.ijk;
		ijk.y *= -1.0f;
		orientation_.set(orientation_.w, ijk.x, ijk.y, ijk.z);
		pushConst.model = MMath::translate(pos) * QMath::toMatrix4(orientation_);
	}
	else {
		pushConst.model.loadIdentity();
	}
	pushConst.normal = MMath::transpose(MMath::inverse(pushConst.model));

	return pushConst;
}

Quaternion CameraActor::GetRotation(){

	Ref<TransformComponent> transform = GetComponent<TransformComponent>();
	Vec3 pos = transform->GetPosition();
	Quaternion orientation_ = transform->GetOrientation();
	Vec3 ijk = orientation_.ijk;
	ijk.y *= -1.0f;
	orientation_.set(orientation_.w, ijk.x, ijk.y, ijk.z);

	return orientation_;
}

void CameraActor::UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far) {
	projectionMatrix = MMath::perspective(fovy, aspectRatio, near, far);
}

