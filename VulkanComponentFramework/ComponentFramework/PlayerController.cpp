#include "PlayerController.h"
#include "Actor.h"
#include "CameraActor.h"


PlayerController::PlayerController(Component* parent_): Component(parent_), pawnActor(nullptr), roomSurvived(0) {

}

void PlayerController::GetPlayerInput(const SDL_Event& Event, TransformComponent* TF_Component)
{	

	Ref<TransformComponent> transform = pawnActor->GetComponent<TransformComponent>();
	//Ref<TransformComponent> transform = TF_Component;
	Ref<CameraActor> camera = pawnActor->GetComponent<CameraActor>();

	Vec3 forward = Vec3(0.0f, 0.0f, -1.0f);
	if (camera) {
		forward = QMath::rotate(forward, camera->GetComponent<TransformComponent>()->GetOrientation());
		//DONT USE CAMERA FORWARD VEC, IT WONT WORK
	}

	if (!transform)
		return;

	// temp var(s)
	float moveSpeed = 0.5f;
	Vec3 pos = transform->GetPosition();
	Quaternion orient = transform->GetOrientation();

	// Key DOWN
	if (KeyDown(KeyCode::W, Event))
	{	
		pos = transform->GetPosition() + (moveSpeed * forward);
		//pos.z -= moveSpeed;
		transform->SetTransform(pos, orient);
	}
	else if (KeyDown(KeyCode::A, Event))
	{
		pos.x -= moveSpeed;
		transform->SetTransform(pos, orient);
	}
	if (KeyDown(KeyCode::S, Event))
	{
		pos = transform->GetPosition() + (moveSpeed * -forward);
		transform->SetTransform(pos, orient);
	}
	else if (KeyDown(KeyCode::D, Event))
	{
		pos.x += moveSpeed;
		transform->SetTransform(pos, orient);
	}
	if (KeyDown(KeyCode::ESC, Event))
	{
		//printf("Key Pressed\n");
	}	

	// KEY UP
	else if (KeyUP(KeyCode::W, Event))
	{
		//printf("Key Released\n");
	}
	if (KeyUP(KeyCode::A, Event))
	{
		//printf("Key Released\n");
	}
	else if (KeyUP(KeyCode::S, Event))
	{
		//printf("Key Released\n");
	}
	if (KeyUP(KeyCode::D, Event))
	{
		//printf("Key Released\n");
	}

	if (camera) {
		camera->UpdateViewMatrix();
	}

}

PlayerController::~PlayerController()
{
	
}

bool PlayerController::OnCreate()
{
	return false;
}

void PlayerController::OnDestroy()
{
		
}
