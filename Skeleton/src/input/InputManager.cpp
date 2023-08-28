#include "InputManager.h"
#include <Vector2D.h>
#include <ConsoleManager.h>

#define MAX_STICK_VALUE 32767
#define STICK_DEADZONE 3276
#define TRIGGER_DEADZONE 1000

namespace Input {

	InputManager::InputManager() : InputManager(true) {}

	InputManager::InputManager(bool closeWithEscape): closeWithEscape(closeWithEscape) {

		kbState_ = SDL_GetKeyboardState(0);

		valid = initialiseJoysticks();

		clearState();
	}

	bool InputManager::Valid() {
		return valid;
	}

	InputManager::~InputManager() {
		removeJoysticks();
	}

	bool InputManager::handleInput(SDL_Event& e) {
		clearState();
		while (SDL_PollEvent(&e)) {
			update(e);

			if ((closeWithEscape && isKeyDown(SDL_SCANCODE_ESCAPE)) || e.type == SDL_QUIT) return false;
		}

		return true;
	}

	void InputManager::clearState() {

		isKeyDownEvent_ = isKeyUpEvent_ = isMouseButtonEventDown_ = false;
		isMouseButtonEventUp_ = isMouseMotionEvent_ = isMouseWheelEvent_ = false;
		isJoystickButtonDownEvent_ = isJoystickButtonUpEvent_ = false; joystickConnected_ = false;
		joystickDisconnected_ = false;
	}

	void InputManager::update(const SDL_Event& event) {

		int numJoysticks = SDL_NumJoysticks();

		// Check for SDL_NumJoysticks() error
		if (numJoysticks < 0) {
			Console::Output::PrintError("SDL Joystick Query", SDL_GetError());
			return;
		}

		// Checking for controllers connections or disconnections
		if (numJoysticksConnected < numJoysticks)
			joystickConnected();
		else if (numJoysticksConnected > numJoysticks)
			joystickDisconnected();

		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown();
			break;
		case SDL_KEYUP:
			onKeyUp();
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

		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);

		mousePos_.setX(mouse_x);
		mousePos_.setY(mouse_y);
	}


	// --------- KB ------------

	void InputManager::onKeyDown() {
		isKeyDownEvent_ = true;
	}

	void InputManager::onKeyUp() {
		isKeyUpEvent_ = true;
	}

	bool InputManager::keyDownEvent() {
		return isKeyDownEvent_;
	}

	bool InputManager::keyUpEvent() {
		return isKeyUpEvent_;
	}

	// Letter
	bool InputManager::isLetterDown(KB_LETTERS l) {
		return keyDownEvent() && kbState_[ConvertToScancode(l)] == 1;
	}

	bool InputManager::isLetterUp(KB_LETTERS l) {
		return keyUpEvent() && kbState_[ConvertToScancode(l)] == 0;
	}

	// Number
	bool InputManager::isNumberDown(KB_NUMBERS n) {
		return keyDownEvent() && kbState_[ConvertToScancode(n)] == 1;
	}

	bool InputManager::isNumberUp(KB_NUMBERS n) {
		return keyUpEvent() && kbState_[ConvertToScancode(n)] == 0;
	}

	// Special Key
	bool InputManager::isSpecialKeyDown(KB_SPECIALKEYS s) {
		return keyDownEvent() && kbState_[ConvertToScancode(s)] == 1;
	}

	bool InputManager::isSpecialKeyUp(KB_SPECIALKEYS s) {
		return keyUpEvent() && kbState_[ConvertToScancode(s)] == 0;
	}

	// With SDL Scancode enum
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

	Utilities::Vector2D InputManager::getMousePos() {
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

	bool InputManager::initialiseJoysticks() {

		int nJoysticks = SDL_NumJoysticks();

		// Check for SDL_NumJoysticks() error
		if (nJoysticks < 0) {
			Console::Output::PrintError("SDL Joystick Initialisation", SDL_GetError());
			return false;
		}

		// Check for SDL_JoystickEventState(SDL_ENABLE) error
		if (SDL_JoystickEventState(SDL_ENABLE) < 0) {
			Console::Output::PrintError("SDL Joystick Initialisation", SDL_GetError());
			return false;
		}

		if (nJoysticks > 0) {

			for (int i = 0; i < nJoysticks; i++) addJoystick(i);

			clearJoysticksButtons();

		}

		numJoysticksConnected = nJoysticks;

		return true;
	}

	bool InputManager::addJoystick(int joystick_id) {

		SDL_Joystick* joy = SDL_JoystickOpen(joystick_id);

		// Check for SDL_JoystickOpen() error
		if (joy == NULL) {
			Console::Output::PrintError("SDL Joystick Initialisation", SDL_GetError());
			return false;
		}

		const char* buffer = SDL_JoystickName(joy);
		std::string joystickName(buffer);

		// Check for SDL_JoystickName() error
		if (buffer == NULL) {
			Console::Output::PrintError("SDL Joystick Initialisation", SDL_GetError());
			return false;
		}

		int joyNButtons = SDL_JoystickNumButtons(joy);
		if (joyNButtons < 0) {
			Console::Output::PrintError("SDL Joystick Initialisation", SDL_GetError());
			return false;
		}

		// Adds the information
		joysticks.push_back(joy);

		joystickNames.push_back(joystickName);

		joystickValues.push_back(std::make_pair(new Utilities::Vector2D(0, 0), new Utilities::Vector2D(0, 0)));

		joystickTriggerValues.push_back(std::make_pair(0, 0));

		joystickNumButtons.push_back(joyNButtons);

		return true;
	}

	void InputManager::removeJoystick(int joystick_id) {
		SDL_JoystickClose(joysticks[joystick_id]);

		joysticks.pop_back();

		joystickNames.pop_back();

		delete joystickValues[joystick_id].first;
		delete joystickValues[joystick_id].second;
		joystickValues.pop_back();

		joystickTriggerValues.pop_back();

		joystickNumButtons.pop_back();
	}

	void InputManager::clearJoysticksButtons() {

		for (auto i = 0u; i < joysticks.size(); i++) {

			int joyNButtons = SDL_JoystickNumButtons(joysticks[i]);
			if (joyNButtons < 0) {
				Console::Output::PrintError("SDL Joystick Initialisation", SDL_GetError());
				return;
			}

			joystickButtonStates.push_back(std::vector<bool>(joyNButtons, false));
		}
	}

	void InputManager::removeJoysticks() {
		for (int i = numJoysticksConnected - 1; i >= 0; i--) removeJoystick(i);
	}

	void InputManager::joystickConnected() {
		joystickConnected_ = true;

		addJoystick(numJoysticksConnected);

		int joyNButtons = SDL_JoystickNumButtons(joysticks[numJoysticksConnected]);
		if (joyNButtons < 0) {
			Console::Output::PrintError("SDL Joystick Initialisation", SDL_GetError());
			return;
		}

		joystickButtonStates.push_back(std::vector<bool>(joyNButtons, false));

		numJoysticksConnected++;
	}

	void InputManager::joystickDisconnected() {
		joystickDisconnected_ = true;

		numJoysticksConnected--;

		removeJoystick(numJoysticksConnected);

		joystickButtonStates.pop_back();
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

					for (auto i = 0u; i < joysticks.size(); i++) {
						joystickValues[joystickId].first->setX(0); joystickValues[joystickId].first->setY(0);
						joystickValues[joystickId].second->setX(0); joystickValues[joystickId].second->setY(0);

						joystickTriggerValues[joystickId].first = 0; joystickTriggerValues[joystickId].second = 0;
					}
				}

			} // Left & right triggers
			else {

				if (event.jaxis.value > -MAX_STICK_VALUE + TRIGGER_DEADZONE) { // Trigger Deadzone

					switch (event.jaxis.axis) {
					case 4: joystickTriggerValues[joystickId].first = event.jaxis.value; break;
					case 5: joystickTriggerValues[joystickId].second = event.jaxis.value; break;
					default:
						break;
					}
				}
				else {
					isAxisMotionEvent_ = false;

					joystickTriggerValues[joystickId].first = 0; joystickTriggerValues[joystickId].second = 0;
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

	int InputManager::getJoysticksConnected() {
		return numJoysticksConnected;
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

	Utilities::Vector2D InputManager::getJoystickValue(CONTROLLERSTICK ct) {

		Utilities::Vector2D v = Utilities::Vector2D();

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

		return joystickValues[joystickId].first->getX() != 0 || joystickValues[joystickId].first->getY() != 0;
	}

	bool InputManager::isRightJoystickMotion() {

		return joystickValues[joystickId].second->getX() != 0 || joystickValues[joystickId].second->getY() != 0;
	}

	bool InputManager::isLeftTriggerMotion() {

		return joystickTriggerValues[joystickId].first != 0;
	}

	bool InputManager::isRightTriggerMotion() {

		return joystickTriggerValues[joystickId].second != 0;
	}

	bool InputManager::joystickConnectedEvent() {

		return joystickConnected_;
	}

	bool InputManager::joystickDisconnectedEvent() {

		return joystickDisconnected_;
	}

	std::string InputManager::getJoystickName() {

		return joystickNames[joystickId];
	}


	// --------- Enum Conversion Methods -----------

	SDL_Scancode InputManager::ConvertToScancode(const KB_LETTERS& letter) {

		SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;

		switch (letter) {
			case A:
				scancode = SDL_SCANCODE_A;
				break;
			case B:
				scancode = SDL_SCANCODE_B;
				break;
			case C:
				scancode = SDL_SCANCODE_C;
				break;
			case D:
				scancode = SDL_SCANCODE_D;
				break;
			case E:
				scancode = SDL_SCANCODE_E;
				break;
			case F:
				scancode = SDL_SCANCODE_F;
				break;
			case G:
				scancode = SDL_SCANCODE_G;
				break;
			case H:
				scancode = SDL_SCANCODE_H;
				break;
			case I:
				scancode = SDL_SCANCODE_I;
				break;
			case J:
				scancode = SDL_SCANCODE_J;
				break;
			case K:
				scancode = SDL_SCANCODE_K;
				break;
			case L:
				scancode = SDL_SCANCODE_L;
				break;
			case M:
				scancode = SDL_SCANCODE_M;
				break;
			case N:
				scancode = SDL_SCANCODE_N;
				break;
			case O:
				scancode = SDL_SCANCODE_O;
				break;
			case P:
				scancode = SDL_SCANCODE_P;
				break;
			case Q:
				scancode = SDL_SCANCODE_Q;
				break;
			case R:
				scancode = SDL_SCANCODE_R;
				break;
			case S:
				scancode = SDL_SCANCODE_S;
				break;
			case T:
				scancode = SDL_SCANCODE_T;
				break;
			case U:
				scancode = SDL_SCANCODE_U;
				break;
			case V:
				scancode = SDL_SCANCODE_V;
				break;
			case W:
				scancode = SDL_SCANCODE_W;
				break;
			case X:
				scancode = SDL_SCANCODE_X;
				break;
			case Y:
				scancode = SDL_SCANCODE_Y;
				break;
			case Z:
				scancode = SDL_SCANCODE_Z;
				break;
			default:
				break;
		}

		return scancode;

	}

	SDL_Scancode InputManager::ConvertToScancode(const KB_NUMBERS& number) {

		SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;

		switch (number) {
			case Num1:
				scancode = SDL_SCANCODE_1;
				break;
			case Num2:
				scancode = SDL_SCANCODE_2;
				break;
			case Num3:
				scancode = SDL_SCANCODE_3;
				break;
			case Num4:
				scancode = SDL_SCANCODE_4;
				break;
			case Num5:
				scancode = SDL_SCANCODE_5;
				break;
			case Num6:
				scancode = SDL_SCANCODE_6;
				break;
			case Num7:
				scancode = SDL_SCANCODE_7;
				break;
			case Num8:
				scancode = SDL_SCANCODE_8;
				break;
			case Num9:
				scancode = SDL_SCANCODE_9;
				break;
			case Num0:
				scancode = SDL_SCANCODE_0;
				break;
			case F1:
				scancode = SDL_SCANCODE_F1;
				break;
			case F2:
				scancode = SDL_SCANCODE_F2;
				break;
			case F3:
				scancode = SDL_SCANCODE_F3;
				break;
			case F4:
				scancode = SDL_SCANCODE_F4;
				break;
			case F5:
				scancode = SDL_SCANCODE_F5;
				break;
			case F6:
				scancode = SDL_SCANCODE_F6;
				break;
			case F7:
				scancode = SDL_SCANCODE_F7;
				break;
			case F8:
				scancode = SDL_SCANCODE_F8;
				break;
			case F9:
				scancode = SDL_SCANCODE_F9;
				break;
			case F10:
				scancode = SDL_SCANCODE_F10;
				break;
			case F11:
				scancode = SDL_SCANCODE_F11;
				break;
			case F12:
				scancode = SDL_SCANCODE_F12;
				break;
			default:
				break;
		}

		return scancode;

	}

	SDL_Scancode InputManager::ConvertToScancode(const KB_SPECIALKEYS& specialKey) {

		SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;

		switch (specialKey) {
			case RETURN:
				scancode = SDL_SCANCODE_RETURN;
				break;
			case ESCAPE:
				scancode = SDL_SCANCODE_ESCAPE;
				break;
			case BACKSPACE:
				scancode = SDL_SCANCODE_BACKSPACE;
				break;
			case TAB:
				scancode = SDL_SCANCODE_TAB;
				break;
			case SPACE:
				scancode = SDL_SCANCODE_SPACE;
				break;
			case RIGHT:
				scancode = SDL_SCANCODE_RIGHT;
				break;
			case LEFT:
				scancode = SDL_SCANCODE_LEFT;
				break;
			case DOWN:
				scancode = SDL_SCANCODE_DOWN;
				break;
			case UP:
				scancode = SDL_SCANCODE_UP;
				break;
			case LCTRL:
				scancode = SDL_SCANCODE_LCTRL;
				break;
			case LSHIFT:
				scancode = SDL_SCANCODE_LSHIFT;
				break;
			case LALT:
				scancode = SDL_SCANCODE_LALT;
				break;
			case RCTRL:
				scancode = SDL_SCANCODE_RCTRL;
				break;
			case RSHIFT:
				scancode = SDL_SCANCODE_RSHIFT;
				break;
			case RALT:
				scancode = SDL_SCANCODE_RALT;
				break;
			default:
				break;
		}

		return scancode;

	}

}