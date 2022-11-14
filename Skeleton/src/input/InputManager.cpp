#include "InputManager.h"

#include <iostream>

namespace InputManager {

	InputManager::InputManager() {

		kbState_ = SDL_GetKeyboardState(0);

		initialiseJoysticks();

		clearState();
	}

	InputManager::~InputManager() {
		removeJoysticks();
	}

	bool InputManager::handleInput(SDL_Event& e) {
		clearState();
		while (SDL_PollEvent(&e)) {
			update(e);

			if (isKeyDown(SDL_SCANCODE_ESCAPE) || e.type == SDL_QUIT) {
				return false;
				continue;
			}
		}

		return true;
	}

	void InputManager::clearState() {

		isKeyDownEvent_ = false;
		isKeyUpEvent_ = false;
		isMouseButtonEventDown_ = false;
		isMouseButtonEventUp_ = false;
		isMouseMotionEvent_ = false;
		isMouseWheelEvent_ = false;
		isJoystickButtonDownEvent_ = false;
		isJoystickButtonUpEvent_ = false;
	}

	void InputManager::update(const SDL_Event& event) {

		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp(event);
			break;
		case SDL_MOUSEWHEEL:
			onMouseWheelMotion(event);
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
		case SDL_JOYAXISMOTION:
			onJoystickAxisMotion(event);
			break;
		case SDL_JOYBUTTONDOWN:
			onJoystickButtonDown(event);
			break;
		case SDL_JOYBUTTONUP:
			onJoystickButtonUp(event);
			break;
		default:
			break;
		}
	}

	// --------- KB ------------

	void InputManager::onKeyDown(const SDL_Event& event) {
		isKeyDownEvent_ = true;
	}

	void InputManager::onKeyUp(const SDL_Event& event) {
		isKeyUpEvent_ = true;
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

	// ----------- MOUSE -----------------

	bool InputManager::mouseMotionEvent() {
		return isMouseMotionEvent_;
	}

	bool InputManager::wheelMotionEvent() {
		return isMouseWheelEvent_;
	}

	const std::pair<Sint32, Sint32>& InputManager::getMousePos() {
		return mousePos_;
	}

	int InputManager::getWheelMotionY() {
		return wheelMotionY_;
	}

	bool InputManager::isMouseButtonDown(MOUSEBUTTON b) {
		return mbState_[b];
	}

	bool InputManager::isMouseButtonUp(MOUSEBUTTON b) {
		return !mbState_[b];
	}

	bool InputManager::isMouseButtonDownEvent(MOUSEBUTTON b) {
		return isMouseButtonEventDown_ && mbState_[b];
	}

	bool InputManager::isMouseButtonUpEvent(MOUSEBUTTON b) {
		return isMouseButtonEventUp_ && !mbState_[b];
	}

	void InputManager::onMouseMotion(const SDL_Event& event) {
		isMouseMotionEvent_ = true;
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;
	}

	void InputManager::onMouseWheelMotion(const SDL_Event& event) {
		wheelMotionY_ = event.wheel.y;
		isMouseWheelEvent_ = true;
	}

	void InputManager::onMouseButtonChange(const SDL_Event& event, bool isDown) {
		if (isDown) isMouseButtonEventDown_ = true;
		else isMouseButtonEventUp_ = true;

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

	// ---------- CONTROLLER -----------

	void InputManager::initialiseJoysticks() {

		int nJoysticks = SDL_NumJoysticks();

		if (nJoysticks > 0) {

			for (int i = 0; i < nJoysticks; i++) {
				SDL_Joystick* joy = SDL_JoystickOpen(i);

				assert(joy != NULL);
					
				joysticks.push_back(joy);

				joystickValues.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0)));

				joystickTriggerValues.push_back(std::make_pair(0, 0));

				joystickNumButtons.push_back(SDL_JoystickNumButtons(joy));
			}

			clearJoysticksButtons();

			SDL_JoystickEventState(SDL_ENABLE);
		}

		numJoysticksConnected = nJoysticks;
	}

	void InputManager::clearJoysticksButtons() {

		for (auto i = 0u; i < joysticks.size(); i++) {
			joystickButtonStates.push_back(std::vector<bool>(SDL_JoystickNumButtons(joysticks[i]), false));
		}
	}

	void InputManager::removeJoysticks() {
		for (int i = 0; i < SDL_NumJoysticks(); i++) {

			SDL_JoystickClose(joysticks[i]);
			delete joystickValues[i].first;
			delete joystickValues[i].second;
		}
	}

	void InputManager::onJoystickAxisMotion(const SDL_Event& event) {

		if (joysticks.size() > 0) {

			isAxisMotionEvent_ = true;
			joystickId = event.jaxis.which;

			// Left & right joysticks
			if (event.jaxis.axis < 4) {

				if (std::abs(event.jaxis.value) > STICK_DEADZONE) // Deadzone
				{
					switch (event.jaxis.axis) {
						case 0: joystickValues[joystickId].first->setX(event.jaxis.value); break;
						case 1: joystickValues[joystickId].first->setY(-event.jaxis.value); break;
						case 2: joystickValues[joystickId].second->setX(event.jaxis.value); break;
						case 3: joystickValues[joystickId].second->setY(-event.jaxis.value); break;
						default: 
							break;
					}
				}
				else {

					isAxisMotionEvent_ = false;

					/*for (auto i = 0u; i < joysticks.size(); i++) {
						joystickValues[joystickId].first->setX(0); joystickValues[joystickId].first->setY(0);
						joystickValues[joystickId].second->setX(0); joystickValues[joystickId].second->setY(0);

						joystickTriggerValues[joystickId].first = 0; joystickTriggerValues[joystickId].second = 0;
					}*/
				}

			} // Left & right triggers
			else {

				if (std::abs(event.jaxis.value) > TRIGGER_DEADZONE) {

					switch (event.jaxis.axis) {
						case 0: joystickTriggerValues[joystickId].first = event.jaxis.value; break;
						case 1: joystickTriggerValues[joystickId].second = event.jaxis.value; break;
						default: 
							break;
					}
				}
				else {
					isAxisMotionEvent_ = false;

					//joystickTriggerValues[joystickId].first = 0; joystickTriggerValues[joystickId].second = 0;
				}
			}
		}
	}

	void InputManager::onJoystickButtonDown(const SDL_Event& event) {
		isJoystickButtonDownEvent_ = true;
		joystickId = event.jaxis.which;

		joystickButtonStates[joystickId][event.jbutton.button] = true;
	}

	void InputManager::onJoystickButtonUp(const SDL_Event& event) {
		isJoystickButtonUpEvent_ = true;
		joystickId = event.jaxis.which;

		joystickButtonStates[joystickId][event.jbutton.button] = false;
	}

	bool InputManager::isJoystickAxisMotion() {
		return isAxisMotionEvent_;
	}

	bool InputManager::isJoystickButtonEventDown() {
		return isJoystickButtonDownEvent_;
	}

	bool InputManager::isJoystickButtonEventUp() {
		return isJoystickButtonUpEvent_;
	}

	bool InputManager::getJoystickId() {
		return joystickId;
	}

	Vector2D InputManager::getJoystickValue(CONTROLLERSTICK ct) {

		Vector2D v = Vector2D();

		switch (ct) {
		case LEFT_STICK:
			v = joystickValues[joystickId].first;
			break;
		case RIGHT_STICK:
			v = joystickValues[joystickId].second;
			break;
		default:
			break;
		}

		return v / (MAX_STICK_VALUE);
	}

	float InputManager::getJoystickTriggerValue(CONTROLLERTRIGGER ct) {

		float v = 0.0f;

		switch (ct) {
		case LEFT_TRIGGER:
			v = joystickTriggerValues[joystickId].first;
			break;
		case RIGHT_TRIGGER:
			v = joystickTriggerValues[joystickId].second;
			break;
		default:
			break;
		}

		return v / (float)(MAX_STICK_VALUE);
	}

	bool InputManager::getJoystickButtonState(int button) {

		return joystickButtonStates[joystickId][button];
	}
	int InputManager::getJoysticksNumButtons() {

		return joystickNumButtons[joystickId];
	}
	bool InputManager::isLeftJoystickMotion() {

		return std::abs(joystickValues[joystickId].first->getX()) > STICK_DEADZONE || std::abs(joystickValues[joystickId].first->getY()) > STICK_DEADZONE;
	}
	bool InputManager::isRightJoystickMotion() {

		return std::abs(joystickValues[joystickId].second->getX()) > STICK_DEADZONE || std::abs(joystickValues[joystickId].second->getY()) > STICK_DEADZONE;
	}
	bool InputManager::isLeftTriggerMotion() {

		return std::abs(joystickTriggerValues[joystickId].first) > TRIGGER_DEADZONE;
	}
	bool InputManager::isRightTriggerMotion() {

		return std::abs(joystickTriggerValues[joystickId].second) > TRIGGER_DEADZONE;
	}
}