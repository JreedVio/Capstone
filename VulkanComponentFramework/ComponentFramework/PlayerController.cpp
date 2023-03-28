#include "PlayerController.h"
#include "Actor.h"
#include "CameraActor.h"
#include "UIManager.h"
#include "AssetManager.h"
#include "Physics.h"
#include "DynamicLinearMovement.h"


using namespace PHYSICS;

PlayerController::PlayerController(Component* parent_, const char* actorName_): Component(parent_), actorName(actorName_), pawnActor(nullptr), roomSurvived(0), isCreated(false) {
	uiManager = UIManager::getInstance();
}

void PlayerController::GetPlayerInput(const SDL_Event& Event)
{	

	Ref<TransformComponent> transform = pawnActor->GetComponent<TransformComponent>();	
	
	Ref<CameraActor> camera = pawnActor->GetComponent<CameraActor>();

	Ref<DynamicLinearMovement> DynamicMovement = pawnActor->GetComponent<DynamicLinearMovement>();

	Vec3 forward = Vec3(0.0f, 0.0f, -1.0f);
	Vec3 upVector = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 rightDirection = Vec3(1.0f, 0.0f, 0.0f);

	if (!transform)
		return;
	
	// temp var(s)
	float moveSpeed = 5.0f;//0.5f;
	Vec3 pos = transform->GetPosition();
	Quaternion orient = transform->GetOrientation();


	//UI events first
	if (KeyPressed(KeyCode::ESC, Event))
	{
		printf("ESC Pressed\n");
		uiManager->openMenu("PauseMenu");
	}

	//When option menu is opened, don't receive any in game input
	if (uiManager->isOpened("PauseMenu")) {
		camera->Deactivate();
		return;
	}


	if (camera) {

		//When option menu is opened, don't receive any in game input
		if (uiManager->isOpened("CodeUI")) {
			camera->Deactivate();
		}
		else {
			//Get forward direction
			forward = QMath::rotate(forward, camera->GetComponent<TransformComponent>()->GetOrientation());
			forward.y = 0.0f;
			rightDirection = VMath::cross(forward, upVector);
			camera->HandleEvents(Event);
		}

	}

	Vec3 dir = Vec3();


	// Key DOWN
	if (KeyDown(KeyCode::W, Event))
	{	
		//pos += (moveSpeed * forward * 0.15f * 0.15f);	
		dir += forward;		
		//pos.z -= moveSpeed;
		//transform->SetTransform(pos, orient);
		
		//printf("W\n");
	}
	if (KeyDown(KeyCode::A, Event))
	{
		//pos += (moveSpeed * -rightDirection * 0.15f * 0.15f);		
		dir += -rightDirection;
		//transform->SetTransform(pos, orient);
		//printf("A\n");
	}
	if (KeyDown(KeyCode::S, Event))
	{
		//pos += (moveSpeed * -forward * 0.15f * 0.15f);		
		dir += -forward;
		//transform->SetTransform(pos, orient);
	}
	if (KeyDown(KeyCode::D, Event))
	{
		//pos.x += moveSpeed;
		//pos += (moveSpeed * rightDirection * 0.15f * 0.15f);		
		dir += rightDirection;
		//transform->SetTransform(pos, orient);
	}

	if (VMath::mag(dir) > VERY_SMALL)
	{
		dir = VMath::normalize(dir);
	}
	DynamicMovement->SetVel(Vec3(moveSpeed * dir.x, DynamicMovement->GetVel().y, moveSpeed * dir.z));

	if (KeyDown(KeyCode::LCTRL, Event)) {
		camera->Deactivate();
	}

	// KEY UP
	if (KeyUP(KeyCode::W, Event))
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
