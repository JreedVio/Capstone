#include "PlayerController.h"
#include "Actor.h"
#include "CameraActor.h"
#include "UIManager.h"
#include "AssetManager.h"
#include "Physics.h"

using namespace PHYSICS;

PlayerController::PlayerController(Component* parent_, const char* actorName_): Component(parent_), actorName(actorName_), pawnActor(nullptr), roomSurvived(0), isCreated(false) {
	uiManager = UIManager::getInstance();
}

void PlayerController::GetPlayerInput(const SDL_Event& Event, TransformComponent* TF_Component)
{	

	Ref<TransformComponent> transform = pawnActor->GetComponent<TransformComponent>();
	//Ref<TransformComponent> transform = TF_Component;
	auto dlm = pawnActor->GetComponent<PHYSICS::DynamicLinearMovement>();

	if (!transform)
		return;

	//Get direction to travel
	Ref<CameraActor> camera = pawnActor->GetComponent<CameraActor>();
	Vec3 forward = Vec3(0.0f, 0.0f, -1.0f);
	Vec3 upVector = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 rightDirection = Vec3(1.0f, 0.0f, 0.0f);
	if (camera) {
		//Get forward direction
		forward = QMath::rotate(forward, camera->GetComponent<TransformComponent>()->GetOrientation());
		forward.y = 0.0f;
		rightDirection = VMath::cross(forward, upVector);
	}

	// temp var(s)
	float moveSpeed = 0.5f;
	Vec3 pos = transform->GetPosition();
	Quaternion orient = transform->GetOrientation();

	//UI events first
	if (KeyDown(KeyCode::ESC, Event))
	{
		//printf("Key Pressed\n");

		uiManager->openMenu("PauseMenu");
	}

	//When option menu is opened, don't receive any in game input
	if (uiManager->isOpened("PauseMenu")) {
		return;
	}
	// Key DOWN
	if (KeyDown(KeyCode::W, Event))
	{	
		pos = transform->GetPosition() + (moveSpeed * forward);
		//pos.z -= moveSpeed;
		transform->SetTransform(pos, orient);
	}
	else if (KeyDown(KeyCode::A, Event))
	{
		pos = transform->GetPosition() + (moveSpeed * -rightDirection);
		transform->SetTransform(pos, orient);
	}
	if (KeyDown(KeyCode::S, Event))
	{
		pos = transform->GetPosition() + (moveSpeed * -forward);
		transform->SetTransform(pos, orient);
	}
	else if (KeyDown(KeyCode::D, Event))
	{
		//pos.x += moveSpeed;
		pos = transform->GetPosition() + (moveSpeed * rightDirection);
		transform->SetTransform(pos, orient);
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

	camera->HandleEvents(Event);
}

PlayerController::~PlayerController()
{
	
}

bool PlayerController::OnCreate(){

	if (isCreated) return false;
	//Create actors and ui elements
	Ref<Actor> actor_ = AssetManager::GetInstance()->GetActor(actorName);
	actor_->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 2.0f, 0.0f), QMath::angleAxisRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f)));
	actor_->OnCreate();
	actor_->SetVisible(false);
	actor_->AddComponent<AABB>(actor_.get(), actor_->GetComponent<TransformComponent>(),
		actor_->GetComponent<TransformComponent>()->GetPosition(), Vec3(1.0f, 1.0f, 1.0f), Quaternion());
	actor_->AddComponent<DynamicLinearMovement>(nullptr, actor_->GetComponent<TransformComponent>());
	actor_->AddComponent<Physics>(nullptr);
	SetPawn(actor_);

	isCreated = true;
	return true;
}

void PlayerController::OnDestroy(){
	isCreated = false;
		
}
