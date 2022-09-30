#include "InputManager.h"

InputManager::InputManager()
{
	kbState_ = SDL_GetKeyboardState(0);
	clearState();
}

void InputManager::clearState() {

	isKeyDownEvent_ = false;
	isKeyUpEvent_ = false;
	isMouseButtonEvent_ = false;
	isMouseMotionEvent_ = false;

	for (auto i = 0u; i < 3; i++) {
		mbState_[i] = false;
	}
}

void InputManager::update(const SDL_Event& event) {

	switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonChange(event, true);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonChange(event, false);
			break;
		default:
			break;
		}
}

bool InputManager::keyDownEvent() {
	return isKeyDownEvent_;
}

bool InputManager::keyUpEvent() {
	return isKeyUpEvent_;
}

bool InputManager::isKeyDown(SDL_Scancode key) {
	return keyDownEvent() && kbState_[key] == 1;
}

bool InputManager::isKeyUp(SDL_Scancode key) {
	return keyUpEvent() && kbState_[key] == 0;
}

bool InputManager::isKeyDown(SDL_Keycode key) {
	return isKeyDown(SDL_GetScancodeFromKey(key));
}

bool InputManager::isKeyUp(SDL_Keycode key) {
	return isKeyUp(SDL_GetScancodeFromKey(key));
}

bool InputManager::mouseMotionEvent() {
	return isMouseMotionEvent_;
}

bool InputManager::mouseButtonEvent() {
	return isMouseButtonEvent_;
}

const std::pair<Sint32, Sint32>& InputManager::getMousePos() {
	return mousePos_;
}

int InputManager::getMouseButtonState(MOUSEBUTTON b) {
	return mbState_[b];
}


// -------------------------------------------------//


void InputManager::onKeyDown(const SDL_Event& event) {
	isKeyDownEvent_ = true;
}

void InputManager::onKeyUp(const SDL_Event& event) {

	isKeyUpEvent_ = true;
}

void InputManager::onMouseMotion(const SDL_Event& event) {
	isMouseMotionEvent_ = true;
	mousePos_.first = event.motion.x;
	mousePos_.second = event.motion.y;
}

void InputManager::onMouseButtonChange(const SDL_Event& event, bool isDown) {
	isMouseButtonEvent_ = true;
	switch (event.button.button) {
	case SDL_BUTTON_LEFT:
		mbState_[LEFT] = isDown;
		break;
	case SDL_BUTTON_MIDDLE:
		mbState_[MIDDLE] = isDown;
		break;
	case SDL_BUTTON_RIGHT:
		mbState_[RIGHT] = isDown;
		break;
	default:
		break;
	}
}