#include "PlayerController.h"



PlayerController::PlayerController(Component* parent_): Component(parent_)
{

}

void PlayerController::GetPlayerInput(const SDL_Event& Event, TransformComponent* TF_Component)
{		
	if (!TF_Component)
		return;

	// temp var(s)	
	float moveSpeed = 0.5f;
	Vec3 pos = TF_Component->GetPosition();
	Quaternion orient = TF_Component->GetOrientation();

	// Key DOWN
	if (KeyDown(KeyCode::W, Event))
	{	
		pos.z -= moveSpeed;
		TF_Component->SetTransform(pos, orient);
	}
	else if (KeyDown(KeyCode::A, Event))
	{
		pos.x -= moveSpeed;
		TF_Component->SetTransform(pos, orient);
	}
	if (KeyDown(KeyCode::S, Event))
	{
		pos.z += moveSpeed;
		TF_Component->SetTransform(pos, orient);
	}
	else if (KeyDown(KeyCode::D, Event))
	{
		pos.x += moveSpeed;
		TF_Component->SetTransform(pos, orient);
	}
	if (KeyDown(KeyCode::ESC, Event))
	{
		printf("Key Pressed\n");
	}	

	// KEY UP
	else if (KeyUP(KeyCode::W, Event))
	{
		printf("Key Released\n");
	}
	if (KeyUP(KeyCode::A, Event))
	{
		printf("Key Released\n");
	}
	else if (KeyUP(KeyCode::S, Event))
	{
		printf("Key Released\n");
	}
	if (KeyUP(KeyCode::D, Event))
	{
		printf("Key Released\n");
	}	

	printf("pos.x: %f, pos.y: %f, pos.z: %f\n", pos.x, pos.y, pos.z);
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
