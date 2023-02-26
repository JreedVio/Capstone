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
	cameraSpeed = 0.45f;
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
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_R) {
			//std::cout << "R EVENT\n";MMath::rotate(5.0f, Vec3(0.0f, 1.0f, 0.0f)) * 
			transform_->SetTransform(
				transform_->GetPosition(), QMath::angleAxisRotation(5.0f, Vec3(0.0f, 1.0f, 0.0f)) *transform_->GetOrientation());
			//if (parent) {
			//	Actor* parentActor = dynamic_cast<Actor*>(parent);
			//	Ref<TransformComponent> parentTransform = parentActor->GetComponent<TransformComponent>();
			//	Matrix4 parentModel = parentActor->GetModelMatrix().model;
			//	//parentModel = MMath::translate(parentTransform->GetPosition() + Vec3(0.0f, 0.0f, -1.0f));
			//	//parentActor->SetPushConst(parentModel);
			//	parentTransform->SetTransform(parentTransform->GetPosition(), QMath::angleAxisRotation(-5.0f, Vec3(0.0f, 1.0f, 0.0f)) * parentTransform->GetOrientation());
			//}
			if (parent) {
				Ref<TransformComponent> parentTransform = dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>();
				parentTransform->SetTransform(parentTransform->GetPosition(),
					QMath::angleAxisRotation(-5.0f, Vec3(0.0f, 1.0f, 0.0f)) * parentTransform->GetOrientation());

				//Quaternion newOreintation = QMath::angleAxisRotation(cosine * cameraSpeed, Vec3(0.0f, 1.0f, 0.0f)) * transform_->GetOrientation();
				//Vec3 ijk = newOreintation.ijk;
				////Make the rotation around z axis fixed
				//newOreintation.set(newOreintation.w, ijk.x, ijk.y, 0.0f);
				//transform_->SetTransform(transform_->GetPosition(), newOreintation);
			}
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_T) {
			//std::cout << "R EVENT\n";MMath::rotate(5.0f, Vec3(0.0f, 1.0f, 0.0f)) * 
			transform_->SetTransform(
				transform_->GetPosition(), QMath::angleAxisRotation(5.0f, Vec3(1.0f, 0.0f, 0.0f)) * transform_->GetOrientation());
			//if (parent) {
			//	Actor* parentActor = dynamic_cast<Actor*>(parent);
			//	Ref<TransformComponent> parentTransform = parentActor->GetComponent<TransformComponent>();
			//	Matrix4 parentModel = parentActor->GetModelMatrix().model;
			//	//parentModel = MMath::translate(parentTransform->GetPosition() + Vec3(0.0f, 0.0f, -1.0f));
			//	//parentActor->SetPushConst(parentModel);
			//	parentTransform->SetTransform(parentTransform->GetPosition(), QMath::angleAxisRotation(-5.0f, Vec3(0.0f, 1.0f, 0.0f)) * parentTransform->GetOrientation());
			//}

			transform_->GetOrientation().print();
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

	case SDL_MOUSEBUTTONDOWN:{
		Vec3 mousedownPos = Vec3(sdlEvent.motion.x, sdlEvent.motion.y, 0.0f);
		lastMousePos = mousedownPos;
		break;
	}
	case SDL_MOUSEMOTION:
		//TODO:Smoother movement (Optional)
		//return;
		/*Find current forward vector of the camera
		* The old quaternion will be take into account at the end, 
		* so this vector does not need to be calculated through current orientation 
		*/
		//return;
		VulkanRenderer* renderer_ = VulkanRenderer::GetInstance();
		float w_ = renderer_->GetWidth();
		float h_ = renderer_->GetHeight();
		Vec3 rotationAxis;
		//Find the destination vector that the current forward vector should rotate to
		Vec3 mousePos = Vec3(sdlEvent.motion.x, sdlEvent.motion.y, 0.0f);

		Vec3 delta = (mousePos - lastMousePos);
		float smoothValue = VERY_SMALL + 10.0f;
		//Limit the camera upgrade interval
		if (VMath::mag(delta) <= smoothValue) {
			break;
		}
		Vec3 current = forwardVec;
		Vec3 cameraPos = transform_->GetPosition();
		if (parent) {
			Ref<TransformComponent> parentTransform = dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>();
			cameraPos = cameraPos - parentTransform->GetPosition();
		}
		//Transform from screen coords to world coords
		Matrix4 ndc = MMath::viewportNDC(w_, h_);
		Matrix4 projection = GetProjectionMatrix();
		Matrix4 view = GetViewMatrix();
		Matrix4 worldTransform = MMath::inverse(ndc * projection * view);
		Vec3 worldMousePos = worldTransform * mousePos;
		Vec3 destination = worldMousePos - cameraPos;
		Vec3 upVector = Vec3(0.0f, 1.0f, 0.0f);
		Vec3 rightDirection = Vec3(-1.0f, 0.0f, 0.0f);
		/*Use the dot products and the magnitude to find the cosine
		* This cosine is usually around 1.0f due to the normalized world coordinates
		* (Really no point to calculate the angle, just do it anyways)
		*/
		float magDesition = VMath::mag(destination);
		float magAxis = VMath::mag(current);
		float cosine = abs(VMath::dot(current, destination) / (magAxis * magDesition));

		//Do separate rotation in x axis and y axis

		if (abs(delta.x) >= VERY_SMALL) {
			if (abs(delta.x) >= w_/2.0f) delta.x /= w_;
			Quaternion newOrientation_ = QMath::angleAxisRotation(delta.x * cameraSpeed, upVector) * transform_->GetOrientation();
			transform_->SetTransform(transform_->GetPosition(), newOrientation_);
			//if (parent) {
			//	Ref<TransformComponent> parentTransform = dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>();
			//	/*parentTransform->SetTransform(parentTransform->GetPosition(), 
			//						QMath::angleAxisRotation(-delta.x * cameraSpeed, upVector) * parentTransform->GetOrientation());
			//*/	transform_->SetTransform(transform_->GetPosition(), transform_->GetOrientation() * parentTransform->GetOrientation());
			//}
		}

		if (abs(delta.y) >= VERY_SMALL) {
			if (abs(delta.y) >= w_ / 2.0f) delta.y /= w_;
			Quaternion newOrientation_ = QMath::angleAxisRotation(delta.y * cameraSpeed, rightDirection) * transform_->GetOrientation();
			Vec3 ijk = newOrientation_.ijk;
			if (ijk.x >= -0.2f && ijk.x <= 0.1f) {
				transform_->SetTransform(transform_->GetPosition(), newOrientation_);
			}

		}

		//Calculate the rotational axis
		rotationAxis = VMath::cross(VMath::normalize(destination), VMath::normalize(current));
		//Use the calculated angle and rotation axis to get the rotation
		//Add the camera velocity as a factor to the angle,
		Quaternion newOreintation = QMath::angleAxisRotation(cosine * cameraSpeed, rotationAxis) * transform_->GetOrientation();
		//Vec3 ijk = newOreintation.ijk;
		//Make the rotation around z axis fixed
		//newOreintation.set(newOreintation.w, ijk.x, ijk.y, 0.0f);


		//Record last mouse position and forward vector
		lastMousePos = mousePos;
		//forwardVec = VMath::normalize(destination);
		//forwardVec = QMath::rotate(Vec3(0.0f, 0.0f, -1.0f), transform_->GetOrientation());
		//Rotate the actor if there's one
		
		//transform_->SetTransform(transform_->GetPosition(), newOreintation);
		newOreintation = transform_->GetOrientation();
		Vec3 ijk = newOreintation.ijk;
		newOreintation.set(newOreintation.w, ijk.x, ijk.y, 0.0f);
		transform_->SetTransform(transform_->GetPosition(), newOreintation);

		transform_->GetOrientation().print();
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

void CameraActor::UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far) {
	projectionMatrix = MMath::perspective(fovy, aspectRatio, near, far);

}