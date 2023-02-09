#pragma once

#include <SDL.h>

enum class KeyCode
{
	// letters
	A = SDL_SCANCODE_A,
	B = SDL_SCANCODE_B,
	C = SDL_SCANCODE_C,
	D = SDL_SCANCODE_D,
	E = SDL_SCANCODE_E,
	F = SDL_SCANCODE_F,
	G = SDL_SCANCODE_G,
	H = SDL_SCANCODE_H,
	I = SDL_SCANCODE_I,
	J = SDL_SCANCODE_J,
	K = SDL_SCANCODE_K,
	L = SDL_SCANCODE_L,
	M = SDL_SCANCODE_M,
	N = SDL_SCANCODE_N,
	O = SDL_SCANCODE_O,
	P = SDL_SCANCODE_P,
	Q = SDL_SCANCODE_Q,
	R = SDL_SCANCODE_R,
	S = SDL_SCANCODE_S,
	T = SDL_SCANCODE_T,
	U = SDL_SCANCODE_U,
	V = SDL_SCANCODE_V,
	W = SDL_SCANCODE_W,
	X = SDL_SCANCODE_X,
	Y = SDL_SCANCODE_Y,
	Z = SDL_SCANCODE_Z,

	// specials
	ESC = SDL_SCANCODE_ESCAPE,
	TAB = SDL_SCANCODE_TAB,
	LSHIFT = SDL_SCANCODE_LSHIFT,
	RSHIFT = SDL_SCANCODE_RSHIFT,
	LCTRL = SDL_SCANCODE_LCTRL,
	RCTRL = SDL_SCANCODE_RCTRL,
	LALT = SDL_SCANCODE_LALT,
	RALT = SDL_SCANCODE_RALT,
	SPACE = SDL_SCANCODE_SPACE,
	ENTER = SDL_SCANCODE_RETURN,
	// if you have a keypad on your keyboard you can optionally use this ENTER key as well
	KP_ENTER = SDL_SCANCODE_RETURN2,
	LWinKey = SDL_SCANCODE_LGUI,
	RWinKey = SDL_SCANCODE_RGUI,

	// numbers
	K0 = SDL_SCANCODE_0,
	K1 = SDL_SCANCODE_1,
	K2 = SDL_SCANCODE_2,
	K3 = SDL_SCANCODE_3,
	K4 = SDL_SCANCODE_4,
	K5 = SDL_SCANCODE_5,
	K6 = SDL_SCANCODE_6,
	K7 = SDL_SCANCODE_7,
	K8 = SDL_SCANCODE_8,
	K9 = SDL_SCANCODE_9,
};

enum class MouseButton
{
	mbLeft = SDL_BUTTON_LEFT, 
	mbRight = SDL_BUTTON_RIGHT, 
	mbMiddle = SDL_BUTTON_MIDDLE,	
};

class Input
{

	
public:

	//SDL_Event InputEvent;	
	
	void GetKeyBoardInput();
	void GetMouseInput();

	bool KeyDown(KeyCode Key, const SDL_Event& Event);
	bool KeyUP(KeyCode Key, const SDL_Event& Event);
	bool MButtonDown(MouseButton Button, const SDL_Event& Event);
	bool MButtonUP(MouseButton Button, const SDL_Event& Event);

	//Probably won't use this, unless we want to add controller support
	void GetControllerInput() {};

};

