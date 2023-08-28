#include "InputManager.h"
#include <Vector2D.h>
#include <ConsoleManager.h>

#include "CheckML.h"

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
	bool InputManager::isLetterDown(int l) {
		return keyDownEvent() && kbState_[ConvertToScancode((KB_LETTERS)l)] == 1;
	}

	bool InputManager::isLetterHold(int l) {
		return kbState_[ConvertToScancode((KB_LETTERS)l)] == 1;
	}

	bool InputManager::isLetterUp(int l) {
		return keyUpEvent() && kbState_[ConvertToScancode((KB_LETTERS)l)] == 0;
	}

	// Number
	bool InputManager::isNumberDown(int n) {
		return keyDownEvent() && kbState_[ConvertToScancode((KB_NUMBERS)n)] == 1;
	}
	bool InputManager::isNumberHold(int n) {
		return kbState_[ConvertToScancode((KB_NUMBERS)n)] == 1;
	}

	bool InputManager::isNumberUp(int n) {
		return keyUpEvent() && kbState_[ConvertToScancode((KB_NUMBERS)n)] == 0;
	}

	// Special Key
	bool InputManager::isSpecialKeyDown(int s) {
		return keyDownEvent() && kbState_[ConvertToScancode((KB_SPECIALKEYS)s)] == 1;
	}

	bool InputManager::isSpecialKeyHold(int s) {
		return kbState_[ConvertToScancode((KB_SPECIALKEYS)s)] == 1;
	}

	bool InputManager::isSpecialKeyUp(int s) {
		return keyUpEvent() && kbState_[ConvertToScancode((KB_SPECIALKEYS)s)] == 0;
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

	bool InputManager::isMouseButtonDown(int b) {
		return mbState_[b];
	}

	bool InputManager::isMouseButtonUp(int b) {
		return !mbState_[b];
	}

	bool InputManager::isMouseButtonDownEvent(int b) {
		return isMouseButtonEventDown_ && mbState_[b];
	}

	bool InputManager::isMouseButtonUpEvent(int b) {
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
			mbState_[(int)MOUSEBUTTON::LEFT] = isDown;
			break;
		case SDL_BUTTON_MIDDLE:
			mbState_[(int)MOUSEBUTTON::MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			mbState_[(int)MOUSEBUTTON::RIGHT] = isDown;
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
		for (int i = numJoysticksConnected - 1; i >= 0; i--) {
			removeJoystick(i);
		}
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


	float InputManager::HorizontalMovement()
	{
		float dir = 0;

		if (isSpecialKeyHold((int)KB_SPECIALKEYS::LEFT) || isLetterDown((int)KB_LETTERS::A))
			dir += -1;

		if (isSpecialKeyHold((int)KB_SPECIALKEYS::LEFT) || isLetterDown((int)KB_LETTERS::D))
			dir += 1;

		//TODO: meter el input de mando

		return dir;
	}

	float InputManager::VerticalMovement()
	{
		float dir = 0;

		if (isSpecialKeyHold((int)KB_SPECIALKEYS::DOWN) || isLetterDown((int)KB_LETTERS::S))
			dir += -1;

		if (isSpecialKeyHold((int)KB_SPECIALKEYS::UP) || isLetterDown((int)KB_LETTERS::W))
			dir += 1;

		//TODO: meter el input de mando

		return dir;
	}


	// With ID

	Utilities::Vector2D InputManager::getJoystickValue(int intct, int id) {

		CONTROLLERSTICK ct = (CONTROLLERSTICK) intct;
		Utilities::Vector2D v = Utilities::Vector2D();

		switch (ct) {
		case CONTROLLERSTICK::LEFT_STICK:
			v = joystickValues[id].first;
			break;
		case CONTROLLERSTICK::RIGHT_STICK:
			v = joystickValues[id].second;
			break;
		default:
			break;
		}

		return v / (MAX_STICK_VALUE);
	}

	float InputManager::getJoystickTriggerValue(int intct, int id) {

		CONTROLLERTRIGGER ct = (CONTROLLERTRIGGER)intct;
		float v = 0.0f;

		switch (ct) {
		case CONTROLLERTRIGGER::LEFT_TRIGGER:
			v = joystickTriggerValues[id].first;
			break;
		case CONTROLLERTRIGGER::RIGHT_TRIGGER:
			v = joystickTriggerValues[id].second;
			break;
		default:
			break;
		}

		return v / (float)(MAX_STICK_VALUE);
	}

	bool InputManager::getJoystickButtonState(int button, int id) {

		return joystickButtonStates[id][button];
	}

	int InputManager::getJoysticksNumButtons(int id) {

		return joystickNumButtons[id];
	}

	bool InputManager::isLeftJoystickMotion(int id) {

		return joystickValues[id].first->getX() != 0 || joystickValues[id].first->getY() != 0;
	}

	bool InputManager::isRightJoystickMotion(int id) {

		return joystickValues[id].second->getX() != 0 || joystickValues[id].second->getY() != 0;
	}

	bool InputManager::isLeftTriggerMotion(int id) {

		return joystickTriggerValues[id].first != 0;
	}

	bool InputManager::isRightTriggerMotion(int id) {

		return joystickTriggerValues[id].second != 0;
	}

	bool InputManager::joystickConnectedEvent() {

		return joystickConnected_;
	}


	// With out ID

	Utilities::Vector2D InputManager::getJoystickValue(int ct) {
		return getJoystickValue(ct, joystickId);
	}

	float InputManager::getJoystickTriggerValue(int ct) {
		return getJoystickTriggerValue(ct, joystickId);
	}

	bool InputManager::getJoystickButtonState(int button) {
		return getJoystickButtonState(button, joystickId);
	}

	int InputManager::getJoysticksNumButtons() {
		return getJoystickButtonState(joystickId);
	}

	bool InputManager::isLeftJoystickMotion() {
		return isLeftJoystickMotion(joystickId);
	}

	bool InputManager::isRightJoystickMotion() {
		return isRightJoystickMotion(joystickId);
	}

	bool InputManager::isLeftTriggerMotion() {
		return isLeftTriggerMotion(joystickId);
	}

	bool InputManager::isRightTriggerMotion() {
		return isRightTriggerMotion(joystickId);
	}


	bool InputManager::joystickDisconnectedEvent() {

		return joystickDisconnected_;
	}

	std::string InputManager::getJoystickName(int id) {

		return joystickNames[id];
	}


	// --------- Enum Conversion Methods -----------

	SDL_Scancode InputManager::ConvertToScancode(const KB_LETTERS& letter) {

		SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;

		switch (letter) {
		case KB_LETTERS::A:
				scancode = SDL_SCANCODE_A;
				break;
			case KB_LETTERS::B:
				scancode = SDL_SCANCODE_B;
				break;
			case KB_LETTERS::C:
				scancode = SDL_SCANCODE_C;
				break;
			case KB_LETTERS::D:
				scancode = SDL_SCANCODE_D;
				break;
			case KB_LETTERS::E:
				scancode = SDL_SCANCODE_E;
				break;
			case KB_LETTERS::F:
				scancode = SDL_SCANCODE_F;
				break;
			case KB_LETTERS::G:
				scancode = SDL_SCANCODE_G;
				break;
			case KB_LETTERS::H:
				scancode = SDL_SCANCODE_H;
				break;
			case KB_LETTERS::I:
				scancode = SDL_SCANCODE_I;
				break;
			case KB_LETTERS::J:
				scancode = SDL_SCANCODE_J;
				break;
			case KB_LETTERS::K:
				scancode = SDL_SCANCODE_K;
				break;
			case KB_LETTERS::L:
				scancode = SDL_SCANCODE_L;
				break;
			case KB_LETTERS::M:
				scancode = SDL_SCANCODE_M;
				break;
			case KB_LETTERS::N:
				scancode = SDL_SCANCODE_N;
				break;
			case KB_LETTERS::O:
				scancode = SDL_SCANCODE_O;
				break;
			case KB_LETTERS::P:
				scancode = SDL_SCANCODE_P;
				break;
			case KB_LETTERS::Q:
				scancode = SDL_SCANCODE_Q;
				break;
			case KB_LETTERS::R:
				scancode = SDL_SCANCODE_R;
				break;
			case KB_LETTERS::S:
				scancode = SDL_SCANCODE_S;
				break;
			case KB_LETTERS::T:
				scancode = SDL_SCANCODE_T;
				break;
			case KB_LETTERS::U:
				scancode = SDL_SCANCODE_U;
				break;
			case KB_LETTERS::V:
				scancode = SDL_SCANCODE_V;
				break;
			case KB_LETTERS::W:
				scancode = SDL_SCANCODE_W;
				break;
			case KB_LETTERS::X:
				scancode = SDL_SCANCODE_X;
				break;
			case KB_LETTERS::Y:
				scancode = SDL_SCANCODE_Y;
				break;
			case KB_LETTERS::Z:
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
		case KB_NUMBERS::Num1:
				scancode = SDL_SCANCODE_1;
				break;
			case KB_NUMBERS::Num2:
				scancode = SDL_SCANCODE_2;
				break;
			case KB_NUMBERS::Num3:
				scancode = SDL_SCANCODE_3;
				break;
			case KB_NUMBERS::Num4:
				scancode = SDL_SCANCODE_4;
				break;
			case KB_NUMBERS::Num5:
				scancode = SDL_SCANCODE_5;
				break;
			case KB_NUMBERS::Num6:
				scancode = SDL_SCANCODE_6;
				break;
			case KB_NUMBERS::Num7:
				scancode = SDL_SCANCODE_7;
				break;
			case KB_NUMBERS::Num8:
				scancode = SDL_SCANCODE_8;
				break;
			case KB_NUMBERS::Num9:
				scancode = SDL_SCANCODE_9;
				break;
			case KB_NUMBERS::Num0:
				scancode = SDL_SCANCODE_0;
				break;
			case KB_NUMBERS::F1:
				scancode = SDL_SCANCODE_F1;
				break;
			case KB_NUMBERS::F2:
				scancode = SDL_SCANCODE_F2;
				break;
			case KB_NUMBERS::F3:
				scancode = SDL_SCANCODE_F3;
				break;
			case KB_NUMBERS::F4:
				scancode = SDL_SCANCODE_F4;
				break;
			case KB_NUMBERS::F5:
				scancode = SDL_SCANCODE_F5;
				break;
			case KB_NUMBERS::F6:
				scancode = SDL_SCANCODE_F6;
				break;
			case KB_NUMBERS::F7:
				scancode = SDL_SCANCODE_F7;
				break;
			case KB_NUMBERS::F8:
				scancode = SDL_SCANCODE_F8;
				break;
			case KB_NUMBERS::F9:
				scancode = SDL_SCANCODE_F9;
				break;
			case KB_NUMBERS::F10:
				scancode = SDL_SCANCODE_F10;
				break;
			case KB_NUMBERS::F11:
				scancode = SDL_SCANCODE_F11;
				break;
			case KB_NUMBERS::F12:
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
		case KB_SPECIALKEYS::RETURN:
				scancode = SDL_SCANCODE_RETURN;
				break;
			case KB_SPECIALKEYS::ESCAPE:
				scancode = SDL_SCANCODE_ESCAPE;
				break;
			case KB_SPECIALKEYS::BACKSPACE:
				scancode = SDL_SCANCODE_BACKSPACE;
				break;
			case KB_SPECIALKEYS::TAB:
				scancode = SDL_SCANCODE_TAB;
				break;
			case KB_SPECIALKEYS::SPACE:
				scancode = SDL_SCANCODE_SPACE;
				break;
			case KB_SPECIALKEYS::RIGHT:
				scancode = SDL_SCANCODE_RIGHT;
				break;
			case KB_SPECIALKEYS::LEFT:
				scancode = SDL_SCANCODE_LEFT;
				break;
			case KB_SPECIALKEYS::DOWN:
				scancode = SDL_SCANCODE_DOWN;
				break;
			case KB_SPECIALKEYS::UP:
				scancode = SDL_SCANCODE_UP;
				break;
			case KB_SPECIALKEYS::LCTRL:
				scancode = SDL_SCANCODE_LCTRL;
				break;
			case KB_SPECIALKEYS::LSHIFT:
				scancode = SDL_SCANCODE_LSHIFT;
				break;
			case KB_SPECIALKEYS::LALT:
				scancode = SDL_SCANCODE_LALT;
				break;
			case KB_SPECIALKEYS::RCTRL:
				scancode = SDL_SCANCODE_RCTRL;
				break;
			case KB_SPECIALKEYS::RSHIFT:
				scancode = SDL_SCANCODE_RSHIFT;
				break;
			case KB_SPECIALKEYS::RALT:
				scancode = SDL_SCANCODE_RALT;
				break;
			default:
				break;
		}

		return scancode;

	}

}