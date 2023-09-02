#include "InputManager.h"
#include <Vector2D.h>
#include <ConsoleManager.h>

#include "CheckML.h"

#include <iostream>

#define MAX_STICK_VALUE 32767
#define STICK_DEADZONE 3276

namespace Input {

	InputManager::InputManager() : InputManager(true, 4, 4, std::vector<int>(), 0, 2) {}

	InputManager::InputManager(bool closeWithEscape, int KB_Jump, int KB_Action, std::vector<int> KB_movements, int CT_Jump, int CT_Action) : closeWithEscape(closeWithEscape) {

		this->KB_Jump = KB_Jump;
		this->KB_Action = KB_Action;
		this->KB_movements = KB_movements;

		this->CT_Jump = CT_Jump;
		this->CT_Action = CT_Action;


		kbState_ = SDL_GetKeyboardState(0);

		valid = initControllers();

		numControllersConnected = 0;

		clearState();
	}

	bool InputManager::Valid() {
		return valid;
	}

	InputManager::~InputManager() {}

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
		isControllerButtonDownEvent_ = isControllerButtonUpEvent_ = false; controllerConnected_ = false;
		controllerDisconnected_ = false;

		for (int i = 0; i < (int)KB_LETTERS::Count; i++)
			UpdateKeyState(letters[i]);
		for (int i = 0; i < (int)KB_NUMBERS::Count; i++)
			UpdateKeyState(numbers[i]);
		for (int i = 0; i < (int)KB_SPECIALKEYS::Count; i++)
			UpdateKeyState(specialKeys[i]);

		updateControllersButtons();

	}

	void InputManager::update(const SDL_Event& event) {

		int numJoysticks = SDL_NumJoysticks();

		// Check for SDL_NumJoysticks() error
		if (numJoysticks < 0) {
			Console::Output::PrintError("SDL Joystick Query", SDL_GetError());
			return;
		}

		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown();

			letterPressed(ConvertToLetter(event.key.keysym.scancode));
			numberPressed(ConvertToNumbers(event.key.keysym.scancode));
			specialKeyPressed(ConvertToSpecialKeys(event.key.keysym.scancode));

			break;
		case SDL_KEYUP:
			onKeyUp();

			letterReleased(ConvertToLetter(event.key.keysym.scancode));
			numberReleased(ConvertToNumbers(event.key.keysym.scancode));
			specialKeyReleased(ConvertToSpecialKeys(event.key.keysym.scancode));

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
		case SDL_CONTROLLERDEVICEADDED:
			addController(event.cdevice.which);
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			removeController(event.cdevice.which);
			break;
		case SDL_JOYBUTTONDOWN:
			controllerButtonPressed(event);
			break;
		case SDL_JOYBUTTONUP:
			controllerButtonReleased(event);
			break;
		case SDL_JOYAXISMOTION:
			onControllerAxisMotion(event);
			break;
		default:
			break;
		}

		readSticksValues();

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

	bool InputManager::AnyKeyPressed() {
		return isKeyDownEvent_;
	}

	bool InputManager::AnyKeyReleased() {
		return isKeyUpEvent_;
	}

	// Letter
	bool InputManager::IsLetterPressed(int l) {

		if (l >= (int) KB_LETTERS::Count || l < 0) {
			Console::Output::PrintError("KeyBoard Input", "The letter with index " + std::to_string(l) + " does not exist");
			return false;
		}

		return letters[l] == ButtonState::JustDown;
	}

	bool InputManager::IsLetterDown(int l) {

		if (l >= (int)KB_LETTERS::Count || l < 0) {
			Console::Output::PrintError("KeyBoard Input", "The letter with index " + std::to_string(l) + " does not exist");
			return false;
		}

		return letters[l] == ButtonState::JustDown || letters[l] == ButtonState::Down;
	}

	bool InputManager::IsLetterUp(int l) {

		if (l >= (int)KB_LETTERS::Count || l < 0) {
			Console::Output::PrintError("KeyBoard Input", "The letter with index " + std::to_string(l) + " does not exist");
			return false;
		}

		return letters[l] == ButtonState::Up || letters[l] == ButtonState::None;
	}

	bool InputManager::IsLetterReleased(int l) {

		if (l >= (int)KB_LETTERS::Count || l < 0) {
			Console::Output::PrintError("KeyBoard Input", "The letter with index " + std::to_string(l) + " does not exist");
			return false;
		}

		return letters[l] == ButtonState::Up;
	}

	// Number
	bool InputManager::IsNumberPressed(int n) {

		if (n >= (int)KB_NUMBERS::Count || n < 0) {
			Console::Output::PrintError("KeyBoard Input", "The number with index " + std::to_string(n) + " does not exist");
			return false;
		}

		return numbers[n] == ButtonState::JustDown;
	}

	bool InputManager::IsNumberDown(int n) {

		if (n >= (int)KB_NUMBERS::Count || n < 0) {
			Console::Output::PrintError("KeyBoard Input", "The number with index " + std::to_string(n) + " does not exist");
			return false;
		}

		return numbers[n] == ButtonState::JustDown || numbers[n] == ButtonState::Down;
	}

	bool InputManager::IsNumberUp(int n) {

		if (n >= (int)KB_NUMBERS::Count || n < 0) {
			Console::Output::PrintError("KeyBoard Input", "The number with index " + std::to_string(n) + " does not exist");
			return false;
		}

		return numbers[n] == ButtonState::Up || numbers[n] == ButtonState::None;
	}

	bool InputManager::IsNumberReleased(int n) {

		if (n >= (int)KB_NUMBERS::Count || n < 0) {
			Console::Output::PrintError("KeyBoard Input", "The number with index " + std::to_string(n) + " does not exist");
			return false;
		}

		return numbers[n] == ButtonState::Up;
	}

	// Special Key
	bool InputManager::IsSpecialKeyPressed(int s) {

		if (s >= (int)KB_SPECIALKEYS::Count || s < 0) {
			Console::Output::PrintError("KeyBoard Input", "The special key with index " + std::to_string(s) + " does not exist");
			return false;
		}

		return specialKeys[s] == ButtonState::JustDown;
	}

	bool InputManager::IsSpecialKeyDown(int s) {

		if (s >= (int)KB_SPECIALKEYS::Count || s < 0) {
			Console::Output::PrintError("KeyBoard Input", "The special key with index " + std::to_string(s) + " does not exist");
			return false;
		}

		return specialKeys[s] == ButtonState::JustDown || specialKeys[s] == ButtonState::Down;
	}

	bool InputManager::IsSpecialKeyUp(int s) {

		if (s >= (int)KB_SPECIALKEYS::Count || s < 0) {
			Console::Output::PrintError("KeyBoard Input", "The special key with index " + std::to_string(s) + " does not exist");
			return false;
		}

		return specialKeys[s] == ButtonState::Up || specialKeys[s] == ButtonState::None;
	}

	bool InputManager::IsSpecialKeyReleased(int s) {

		if (s >= (int)KB_SPECIALKEYS::Count || s < 0) {
			Console::Output::PrintError("KeyBoard Input", "The special key with index " + std::to_string(s) + " does not exist");
			return false;
		}

		return specialKeys[s] == ButtonState::Up;
	}

	// With SDL Scancode enum
	bool InputManager::isKeyDown(SDL_Scancode key) {
		return AnyKeyPressed() && kbState_[key] == 1;
	}

	bool InputManager::isKeyUp(SDL_Scancode key) {
		return AnyKeyReleased() && kbState_[key] == 0;
	}



	void InputManager::letterPressed(KB_LETTERS letter)
	{
		if (letter == KB_LETTERS::Count) return;
		if (letters[(int)letter] == ButtonState::Down) return;

		letters[(int)letter] = ButtonState::JustDown;
	}

	void InputManager::numberPressed(KB_NUMBERS number)
	{
		if (number == KB_NUMBERS::Count) return;
		if (numbers[(int)number] == ButtonState::Down) return;

		numbers[(int)number] = ButtonState::JustDown;
	}

	void InputManager::specialKeyPressed(KB_SPECIALKEYS specialKey)
	{
		if (specialKey == KB_SPECIALKEYS::Count) return;
		if (specialKeys[(int)specialKey] == ButtonState::Down) return;

		specialKeys[(int)specialKey] = ButtonState::JustDown;
	}

	void InputManager::letterReleased(KB_LETTERS letter)
	{
		if (letter == KB_LETTERS::Count) return;
		letters[(int)letter] = ButtonState::Up;
	}

	void InputManager::numberReleased(KB_NUMBERS number)
	{
		if (number == KB_NUMBERS::Count) return;
		numbers[(int)number] = ButtonState::Up;
	}

	void InputManager::specialKeyReleased(KB_SPECIALKEYS specialKey)
	{
		if (specialKey == KB_SPECIALKEYS::Count) return;
		specialKeys[(int)specialKey] = ButtonState::Up;
	}


	void InputManager::UpdateKeyState(ButtonState& key)
	{
		if (key == ButtonState::JustDown)
			key = ButtonState::Down;

		if (key == ButtonState::Up)
			key = ButtonState::None;
	}

	float InputManager::HorizontalMovement()
	{
		float dir = 0;

		if (InputManager::ConnectedControllersCount() > 0)
			dir += GetLeftStickHorizontalValue();

		if (IsSpecialKeyDown((int)KB_SPECIALKEYS::LEFT) || IsLetterDown(KB_movements[(int) DIRECTIONS::LEFT]))
		{
			dir += -1;
			if (dir < -1)
				dir = -1;
		}

		if (IsSpecialKeyDown((int)KB_SPECIALKEYS::RIGHT) || IsLetterDown(KB_movements[(int) DIRECTIONS::RIGHT]))
		{
			dir += 1;
			if (dir > 1)
				dir = 1;
		}


		return dir;
	}

	float InputManager::VerticalMovement()
	{
		float dir = 0;

		if (InputManager::ConnectedControllersCount() > 0)
			dir += GetLeftStickVerticalValue();

		if (IsSpecialKeyDown((int)KB_SPECIALKEYS::DOWN) || IsLetterDown(KB_movements[(int) DIRECTIONS::DOWN]))
		{
			dir += -1;
			if (dir < -1)
				dir = -1;
		}

		if (IsSpecialKeyDown((int)KB_SPECIALKEYS::UP) || IsLetterDown(KB_movements[(int) DIRECTIONS::UP]))
		{
			dir += 1;
			if (dir > 1)
				dir = 1;
		}


		return dir;
	}

	bool InputManager::Jump() {
		return IsSpecialKeyPressed(KB_Jump) || IsControllerButtonPressed(CT_Jump);
	}

	bool InputManager::Action() {
		return IsLetterPressed(KB_Action) || IsControllerButtonPressed(CT_Action);
	}


	// ----------- MOUSE -----------------

	bool InputManager::HasMouseMoved() {
		return isMouseMotionEvent_;
	}

	bool InputManager::HasMouseWheelMoved() {
		return isMouseWheelEvent_;
	}

	cVector2D InputManager::GetMousePosition() {
		return mousePos_;
	}

	int InputManager::GetMouseWheelScroll() {
		return wheelMotionY_;
	}

	bool InputManager::IsMouseButtonDown(int b) {

		if (b >= (int)MOUSEBUTTON::Count || b < 0) {
			Console::Output::PrintError("Mouse Input", "The mouse button with index " + std::to_string(b) + " does not exist");
			return false;
		}

		return mbState_[b];
	}

	bool InputManager::IsMouseButtonUp(int b) {

		if (b >= (int)MOUSEBUTTON::Count || b < 0) {
			Console::Output::PrintError("Mouse Input", "The mouse button with index " + std::to_string(b) + " does not exist");
			return false;
		}

		return !mbState_[b];
	}

	bool InputManager::IsMouseButtonPressed(int b) {

		if (b >= (int)MOUSEBUTTON::Count || b < 0) {
			Console::Output::PrintError("Mouse Input", "The mouse button with index " + std::to_string(b) + " does not exist");
			return false;
		}

		return isMouseButtonEventDown_ && mbState_[b];
	}

	bool InputManager::IsMouseButtonReleased(int b) {

		if (b >= (int)MOUSEBUTTON::Count || b < 0) {
			Console::Output::PrintError("Mouse Input", "The mouse button with index " + std::to_string(b) + " does not exist");
			return false;
		}

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

	bool InputManager::initControllers() {

		// Check for SDL_JoystickEventState(SDL_ENABLE) error
		if (SDL_JoystickEventState(SDL_ENABLE) < 0) {
			Console::Output::PrintError("SDL GameController Initialisation", SDL_GetError());
			return false;
		}

		return true;
	}

	void InputManager::addController(int which) {

		controllerConnected_ = true;

		if (!SDL_IsGameController(which))
			return;

		// SDL GameController
		SDL_GameController* sdlController = SDL_GameControllerOpen(which);

		if (sdlController == NULL) {
			Console::Output::PrintError("SDL GameController Initialisation", SDL_GetError());
			return;
		}

		// SDL Joystick
		SDL_Joystick* joystick = SDL_GameControllerGetJoystick(sdlController);

		if (joystick == NULL) {
			Console::Output::PrintError("SDL GameController Initialisation", SDL_GetError());
			return;
		}

		// SDL Joystick ID
		SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(sdlController));

		if (id < 0) {
			Console::Output::PrintError("SDL GameController Initialisation", SDL_GetError());
			return;
		}

		// SDL GameController Name
		const char* buffer = SDL_GameControllerName(sdlController);
		std::string name = "Empty";
		if (buffer != NULL)
			name = buffer;


		// Controller Data
		ControllerData data;
		data.controller = sdlController;
		data.joystick = joystick;
		data.id = id;
		data.name = name;
		data.buttonsStates = std::vector<ButtonState>((int)PS4_CONTROLLER_BUTTONS::Count, ButtonState::None);
		data.triggers = Utilities::Vector2D();
		data.joysticks = std::make_pair(Utilities::Vector2D(), Utilities::Vector2D());

		controllers.insert(std::make_pair(which, data));

		numControllersConnected++;

	}

	void InputManager::removeController(int which) {

		controllerDisconnected_ = true;

		SDL_GameControllerClose(controllers[which].controller);

		controllers.erase(which);

		numControllersConnected--;
	}




	void InputManager::controllerButtonPressed(const SDL_Event& event) {
		isControllerButtonDownEvent_ = true;
		lastInputReceivedController = event.cdevice.which;

		auto& state = controllers[lastInputReceivedController].buttonsStates[event.cbutton.button];

		if (state == ButtonState::Down) return;

		state = ButtonState::JustDown;
	}

	void InputManager::controllerButtonReleased(const SDL_Event& event) {
		isControllerButtonUpEvent_ = true;
		lastInputReceivedController = event.cdevice.which;

		controllers[lastInputReceivedController].buttonsStates[event.cbutton.button] = ButtonState::Up;
	}

	void InputManager::updateControllersButtons() {

		for (auto& c : controllers) {
			for (auto& b : c.second.buttonsStates) {

				if (b == ButtonState::JustDown)
					b = ButtonState::Down;

				if (b == ButtonState::Up)
					b = ButtonState::None;
			}
		}
	}



	void InputManager::onControllerAxisMotion(const SDL_Event& event) {

		isAxisMotionEvent_ = true;
		lastInputReceivedController = event.cdevice.which;

		if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
			controllers[lastInputReceivedController].triggers.setX((float)event.caxis.value / MAX_STICK_VALUE);

		}
		else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
			controllers[lastInputReceivedController].triggers.setY((float)event.caxis.value / MAX_STICK_VALUE);
		}
	}

	void InputManager::readSticksValues() {

		for (auto& c : controllers) {

			// Left stick
			Sint16 axis0 = SDL_GameControllerGetAxis(c.second.controller, SDL_CONTROLLER_AXIS_LEFTX);
			Sint16 axis1 = SDL_GameControllerGetAxis(c.second.controller, SDL_CONTROLLER_AXIS_LEFTY);

			if (std::abs(axis0) < STICK_DEADZONE)
				axis0 = 0;

			if (std::abs(axis1) < STICK_DEADZONE)
				axis1 = 0;

			c.second.joysticks.first.setX((float)axis0 / MAX_STICK_VALUE);
			c.second.joysticks.first.setY(-(float)axis1 / MAX_STICK_VALUE);


			// Rigth stick
			axis0 = SDL_GameControllerGetAxis(c.second.controller, SDL_CONTROLLER_AXIS_RIGHTX);
			axis1 = SDL_GameControllerGetAxis(c.second.controller, SDL_CONTROLLER_AXIS_RIGHTY);

			if (std::abs(axis0) < STICK_DEADZONE)
				axis0 = 0;

			if (std::abs(axis1) < STICK_DEADZONE)
				axis1 = 0;

			c.second.joysticks.second.setX((float)axis0 / MAX_STICK_VALUE);
			c.second.joysticks.second.setY(-(float)axis1 / MAX_STICK_VALUE);

		}


	}

	int InputManager::ConnectedControllersCount() {
		return numControllersConnected;
	}

	bool InputManager::AnyControllerButtonPressed() {
		return isControllerButtonDownEvent_;
	}

	bool InputManager::AnyControllerButtonReleased() {
		return isControllerButtonUpEvent_;
	}

	bool InputManager::AnyControllerAxisMotion() {
		return isAxisMotionEvent_;
	}

	bool InputManager::AnyControllerConnected() {
		return controllerConnected_;
	}

	bool InputManager::AnyControllerDisconnected() {
		return controllerDisconnected_;
	}



	// With ID

	bool InputManager::IsControllerButtonPressedWithId(int button, int id) {

		if (numControllersConnected == 0) return false;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		if (button >= (int)PS4_CONTROLLER_BUTTONS::Count || button < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller button with the specified id: " + std::to_string(button));
			return false;
		}

		return controllers[id].buttonsStates[button] == ButtonState::JustDown;
	}

	bool InputManager::IsControllerButtonDownWithId(int button, int id) {

		if (numControllersConnected == 0) return false;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		if (button >= (int)PS4_CONTROLLER_BUTTONS::Count || button < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller button with the specified id: " + std::to_string(button));
			return false;
		}

		return controllers[id].buttonsStates[button] == ButtonState::JustDown || controllers[id].buttonsStates[button] == ButtonState::Down;
	}

	bool InputManager::IsControllerButtonReleasedWithId(int button, int id) {

		if (numControllersConnected == 0) return false;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		if (button >= (int)PS4_CONTROLLER_BUTTONS::Count || button < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller button with the specified id: " + std::to_string(button));
			return false;
		}

		return controllers[id].buttonsStates[button] == ButtonState::Up;
	}

	float InputManager::GetLeftTriggerValueWithId(int id) {

		if (numControllersConnected == 0) return 0;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].triggers.getX();
	}

	float InputManager::GetRightTriggerValueWithId(int id) {

		if (numControllersConnected == 0) return 0;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].triggers.getY();
	}

	bool InputManager::IsLeftTriggerDownWithId(int id) {

		if (numControllersConnected == 0) return false;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].triggers.getX() > 0;
	}

	bool InputManager::IsRightTriggerDownWithId(int id) {

		if (numControllersConnected == 0) return false;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].triggers.getY() > 0;
	}

	float InputManager::GetLeftStickHorizontalValueWithId(int id) {

		if (numControllersConnected == 0) return 0;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].joysticks.first.getX();
	}

	float InputManager::GetLeftStickVerticalValueWithId(int id) {

		if (numControllersConnected == 0) return 0;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].joysticks.first.getY();
	}

	float InputManager::GetRightStickHorizontalValueWithId(int id) {

		if (numControllersConnected == 0) return 0;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].joysticks.second.getX();
	}

	float InputManager::GetRightStickVerticalValueWithId(int id) {

		if (numControllersConnected == 0) return 0;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].joysticks.second.getY();
	}

	bool InputManager::HasLeftStickMovedWithId(int id) {

		if (numControllersConnected == 0) return false;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].joysticks.first.getX() > 0 || controllers[id].joysticks.first.getY() > 0;
	}

	bool InputManager::HasRightStickMovedWithId(int id) {

		if (numControllersConnected == 0) return false;

		if (id >= numControllersConnected || id < 0) {
			Console::Output::PrintWarning("Controller Input", "There is no controller with the specified id: " + std::to_string(id));
			return false;
		}

		return controllers[id].joysticks.second.getX() > 0 || controllers[id].joysticks.second.getY() > 0;
	}



	// With out ID

	bool InputManager::IsControllerButtonPressed(int button) {
		return IsControllerButtonPressedWithId(button, lastInputReceivedController);
	}

	bool InputManager::IsControllerButtonDown(int button) {
		return IsControllerButtonDownWithId(button, lastInputReceivedController);
	}

	bool InputManager::IsControllerButtonReleased(int button) {
		return IsControllerButtonReleasedWithId(button, lastInputReceivedController);
	}

	float InputManager::GetLeftTriggerValue() {
		return GetLeftTriggerValueWithId(lastInputReceivedController);
	}

	float InputManager::GetRightTriggerValue() {
		return GetRightTriggerValueWithId(lastInputReceivedController);
	}

	bool InputManager::IsLeftTriggerDown() {
		return IsLeftTriggerDownWithId(lastInputReceivedController);
	}

	bool InputManager::IsRightTriggerDown() {
		return IsRightTriggerDownWithId(lastInputReceivedController);
	}

	float InputManager::GetLeftStickHorizontalValue() {
		return GetLeftStickHorizontalValueWithId(lastInputReceivedController);
	}

	float InputManager::GetLeftStickVerticalValue() {
		return GetLeftStickVerticalValueWithId(lastInputReceivedController);
	}

	float InputManager::GetRightStickHorizontalValue() {
		return GetRightStickHorizontalValueWithId(lastInputReceivedController);
	}

	float InputManager::GetRightStickVerticalValue() {
		return GetRightStickVerticalValueWithId(lastInputReceivedController);
	}

	bool InputManager::HasLeftStickMoved() {
		return HasLeftStickMovedWithId(lastInputReceivedController);
	}

	bool InputManager::HasRightStickMoved() {
		return HasRightStickMovedWithId(lastInputReceivedController);
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

	Input::InputManager::KB_LETTERS Input::InputManager::ConvertToLetter(const SDL_Scancode& scancode)
	{
		if (scancode >= SDL_SCANCODE_A && scancode <= SDL_SCANCODE_Z) {

			return (KB_LETTERS)(scancode - SDL_SCANCODE_A);
		}

		return KB_LETTERS::Count;
	}

	Input::InputManager::KB_NUMBERS Input::InputManager::ConvertToNumbers(const SDL_Scancode& scancode) {

		if (scancode >= SDL_SCANCODE_1 && scancode <= SDL_SCANCODE_0) {

			return (KB_NUMBERS)(scancode - SDL_SCANCODE_1);
		}

		if (scancode >= SDL_SCANCODE_F1 && scancode <= SDL_SCANCODE_F12) {

			return (KB_NUMBERS)((int)KB_NUMBERS::F1 + scancode - SDL_SCANCODE_F1);
		}

		return KB_NUMBERS::Count;
	}

	Input::InputManager::KB_SPECIALKEYS Input::InputManager::ConvertToSpecialKeys(const SDL_Scancode& scancode)
	{
		switch (scancode) {
		case SDL_SCANCODE_RETURN:
			return KB_SPECIALKEYS::RETURN;
		case SDL_SCANCODE_ESCAPE:
			return KB_SPECIALKEYS::ESCAPE;
		case SDL_SCANCODE_BACKSPACE:
			return KB_SPECIALKEYS::BACKSPACE;
		case SDL_SCANCODE_TAB:
			return KB_SPECIALKEYS::TAB;
		case SDL_SCANCODE_SPACE:
			return KB_SPECIALKEYS::SPACE;
		case SDL_SCANCODE_RIGHT:
			return KB_SPECIALKEYS::RIGHT;
		case SDL_SCANCODE_LEFT:
			return  KB_SPECIALKEYS::LEFT;
		case SDL_SCANCODE_DOWN:
			return KB_SPECIALKEYS::DOWN;
		case SDL_SCANCODE_UP:
			return KB_SPECIALKEYS::UP;
		case SDL_SCANCODE_LCTRL:
			return KB_SPECIALKEYS::LCTRL;
		case SDL_SCANCODE_LSHIFT:
			return KB_SPECIALKEYS::LSHIFT;
		case SDL_SCANCODE_LALT:
			return KB_SPECIALKEYS::LALT;
		case SDL_SCANCODE_RCTRL:
			return KB_SPECIALKEYS::RCTRL;
		case SDL_SCANCODE_RSHIFT:
			return KB_SPECIALKEYS::RSHIFT;
		case SDL_SCANCODE_RALT:
			return KB_SPECIALKEYS::RALT;
		default:
			break;
		}

		return KB_SPECIALKEYS::Count;
	}

}