#pragma once

class uint8_t;
struct SDL_Event;
struct SDL_Keycode;
struct SDL_Scancode;

class ImVec2;

enum MOUSEBUTTON {
	LEFT = 0, MIDDLE = 1, RIGHT = 2
};

class InputManager
{
public:

	// Called in the main loop to check is window is closed
	bool handleInput(SDL_Event& e);

	// keyboard
	bool keyDownEvent();

	bool keyUpEvent();

	bool isKeyDown(SDL_Scancode key);

	bool isKeyDown(SDL_Keycode key);

	bool isKeyUp(SDL_Scancode key);

	bool isKeyUp(SDL_Keycode key);

	// mouse
	bool mouseMotionEvent();

	bool mouseButtonEventDown();

	bool mouseButtonEventUp();

	bool isMouseButtonDown(MOUSEBUTTON b);

	bool isMouseButtonUp(MOUSEBUTTON b);

	ImVec2 getMousePos();
};