#include "Input.h"






void Input::GetKeyBoardInput()
{

	

}

void Input::GetMouseInput()
{

	

}

bool Input::KeyDown(KeyCode Key, const SDL_Event& Event)
{	 	 
	if (Event.type == SDL_KEYDOWN && Event.key.keysym.scancode == (SDL_Scancode)Key) { return true; }
	else { return false; }
}

bool Input::KeyUP(KeyCode Key, const SDL_Event& Event)
{
	if (Event.type == SDL_KEYUP && Event.key.keysym.scancode == (SDL_Scancode)Key) { return true; }
	else { return false; }
}

bool Input::MButtonDown(MouseButton Button, const SDL_Event& Event)
{
	if (Event.type == SDL_MOUSEBUTTONDOWN && Event.button.button == (Uint8)Button) { return true; }
	else { return false; }
}

bool Input::MButtonUP(MouseButton Button , const SDL_Event& Event)
{
	if (Event.type == SDL_MOUSEBUTTONUP && Event.button.button == (Uint8)Button) { return true; }
	else { return false; }
}
