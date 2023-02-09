#include "PlayerController.h"



PlayerController::PlayerController(Component* parent_): Component(parent_)
{

}

void PlayerController::GetPlayerInput(const SDL_Event& Event)
{		
	
	// Key DOWN
	if (KeyDown(KeyCode::W, Event))
	{		
		printf("Key Pressed\n");
	}
	else if (KeyDown(KeyCode::A, Event))
	{
		printf("Key Pressed\n");
	}
	if (KeyDown(KeyCode::S, Event))
	{
		printf("Key Pressed\n");
	}
	else if (KeyDown(KeyCode::D, Event))
	{
		printf("Key Pressed\n");
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
