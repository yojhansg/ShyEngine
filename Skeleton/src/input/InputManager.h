#pragma once

#include "SDL.h"

#include <unordered_map>
#include <Singleton.h>
#include <Vector2D.h>
#include <vector>
#include <string>
#include <array>

#include <EditorExport.h>

/*
Input Manager. It has support for mouse, keyboard and multiple PS4 controllers.
*/

namespace Utilities {
	class Vector2D;
}

namespace Input {

	EditorManager InputManager : public Utilities::Singleton<InputManager> {

		friend Singleton<InputManager>;

	public:

		enum class MOUSEBUTTON : uint8_t {
			LEFT = 0, MIDDLE = 1, RIGHT = 2
		};

		enum class CONTROLLERSTICK : uint8_t {
			LEFT_STICK = 0, RIGHT_STICK = 1
		};

		enum class CONTROLLERTRIGGER : uint8_t {
			LEFT_TRIGGER = 0, RIGHT_TRIGGER = 1, N_TRIGGERS = 2
		};

		// PS4 Mapping
		enum class PS4_CONTROLLER_BUTTONS : uint8_t {
			X = 0, CIRCLE = 1, SQUARE = 2, TRIANGLE = 3, SHARE = 4, HOME = 5, OPTIONS = 6, L3 = 7, R3 = 8,
			L1 = 9, R1 = 10, UP_ARROW = 11, DOWN_ARROW = 12, LEFT_ARROW = 13, RIGHT_ARROW = 14, PANEL = 15, Count = 16
		};

		// XBOX Mapping
		enum class XBOX_CONTROLLER_BUTTONS : uint8_t {
			A = 0, B = 1, X = 2, Y = 3, SHARE = 4, HOME = 5, OPTIONS = 6, L3 = 7, R3 = 8,
			L1 = 9, R1 = 10, UP_ARROW = 11, DOWN_ARROW = 12, LEFT_ARROW = 13, RIGHT_ARROW = 14, PANEL = 15
		};

		enum class ButtonState {
			None, JustDown, Down, Up
		};

		// Keyboard

		enum class KB_LETTERS {
			A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7, I = 8, J = 9, K = 10, L = 11, M = 12, N = 13, O = 14, P = 15,
			Q = 16, R = 17, S = 18, T = 19, U = 20, V = 21, W = 22, X = 23, Y = 24, Z = 25, Count
		};

		enum class KB_NUMBERS {
			Num1 = 0, Num2 = 1, Num3 = 2, Num4 = 3, Num5 = 4, Num6 = 5, Num7 = 6, Num8 = 7, Num9 = 8, Num0 = 9, F1 = 10, F2 = 11, 
			F3 = 12, F4 = 13, F5 = 14, F6 = 15, F7 = 16, F8 = 17, F9 = 18, F10 = 19, F11 = 20, F12 = 21, Count
		};

		enum class KB_SPECIALKEYS {
			RETURN = 0, ESCAPE = 1, BACKSPACE = 2, TAB = 3, SPACE = 4, RIGHT = 5, LEFT = 6, DOWN = 7, UP = 8, LCTRL = 9, LSHIFT = 10,
			LALT = 11, RCTRL = 12, RSHIFT = 13, RALT = 14, Count
		};

		~InputManager();

		bool Valid() override;

		// Called in the main loop to check is window is closed
		bool handleInput(SDL_Event& e);

		bool isKeyDown(SDL_Scancode key);

		bool isKeyUp(SDL_Scancode key);

		void UpdateKeyState(ButtonState& key);

	publish:

		// Keyboard
		bool keyDownEvent();
		bool keyUpEvent();

		bool isLetterDown(int l);
		bool isLetterHold(int l);
		bool isLetterUp(int l);

		bool isNumberDown(int n);
		bool isNumberHold(int n);
		bool isNumberUp(int n);

		bool isSpecialKeyDown(int s);
		bool isSpecialKeyHold(int s);
		bool isSpecialKeyUp(int s);

		float KeyBoardHorizontalMovement();
		float KeyBoardVerticalMovement();


		// Mouse
		bool mouseMotionEvent();

		bool wheelMotionEvent();

		bool isMouseButtonDown(int b);

		bool isMouseButtonUp(int b);

		bool isMouseButtonDownEvent(int b);

		bool isMouseButtonUpEvent(int b);

		cVector2D getMousePos();

		int getWheelMotionY();


		// Controller
		int getControllersConnected();

		bool isControllersButtonEventDown();

		bool isControllersButtonEventUp();

		bool isControllerAxisMotion();

		bool controllerConnectedEvent();

		bool controllerDisconnectedEvent();

		// With id

			bool isControllerButtonDownWithId(int button, int id);

			bool isControllerButtonHoldWithId(int button, int id);

			bool isControllerButtonUpWithId(int button, int id);

			float getLeftTriggerValueWithId(int id);

			float getRightTriggerValueWithId(int id);

			bool isLeftTriggerMotionWithId(int id);

			bool isRightTriggerMotionWithId(int id);

			float getLeftJoystickXWithId(int id);

			float getLeftJoystickYWithId(int id);

			float getRightJoystickXWithId(int id);

			float getRightJoystickYWithId(int id);

			bool isLeftJoystickMotionWithId(int id);

			bool isRightJoystickMotionWithId(int id);


			float ControllerHorizontalMovementWithId(int id);

			float ControllerVerticalMovementWithId(int id);


		// Without id

			bool isControllerButtonDown(int button);

			bool isControllerButtonHold(int button);

			bool isControllerButtonUp(int button);

			float getLeftTriggerValue();

			float getRightTriggerValue();

			bool isLeftTriggerMotion();

			bool isRightTriggerMotion();

			float getLeftJoystickX();

			float getLeftJoystickY();

			float getRightJoystickX();

			float getRightJoystickY();

			bool isLeftJoystickMotion();

			bool isRightJoystickMotion();


			float ControllerHorizontalMovement();

			float ControllerVerticalMovement();


	private:

		InputManager();
		InputManager(bool closeWithEscape);

		SDL_Scancode ConvertToScancode(const KB_LETTERS& letter);
		SDL_Scancode ConvertToScancode(const KB_NUMBERS& number);
		SDL_Scancode ConvertToScancode(const KB_SPECIALKEYS& specialKey);

		KB_LETTERS ConvertToLetter(const SDL_Scancode& scancode);
		KB_NUMBERS ConvertToNumbers(const SDL_Scancode& scancode);
		KB_SPECIALKEYS ConvertToSpecialKeys(const SDL_Scancode& scancode);


		// Clear the state
		void clearState();

		// Update the state with a new event
		void update(const SDL_Event& event);

		// --------- MOUSE & KB ------------

		void letterPressed(KB_LETTERS letter);
		void numberPressed(KB_NUMBERS letter);
		void specialKeyPressed(KB_SPECIALKEYS letter);

		void letterReleased(KB_LETTERS letter);
		void numberReleased(KB_NUMBERS letter);
		void specialKeyReleased(KB_SPECIALKEYS letter);

		void onKeyDown();

		void onKeyUp();

		void onMouseMotion(const SDL_Event& event);

		void onMouseButtonChange(const SDL_Event& event, bool isDown);

		void onMouseWheelMotion(const SDL_Event& event);


		bool closeWithEscape;
		bool valid;

		bool isKeyUpEvent_;
		bool isKeyDownEvent_;

		bool isMouseMotionEvent_;
		bool isMouseWheelEvent_;
		bool isMouseButtonEventDown_;
		bool isMouseButtonEventUp_;


		Utilities::Vector2D mousePos_;
		std::array<bool, 3> mbState_;
		int wheelMotionY_;
		const Uint8* kbState_;


		// Keyboard keys
		ButtonState letters[(int)KB_LETTERS::Count];
		ButtonState numbers[(int)KB_NUMBERS::Count];
		ButtonState specialKeys[(int)KB_SPECIALKEYS::Count];


		// ----- CONTROLLER -------

		struct ControllerData {
			SDL_GameController* controller;
			SDL_Joystick* joystick;
			SDL_JoystickID id;
			std::string name;
			std::vector<ButtonState> buttonsStates;
			Utilities::Vector2D triggers; // X for LEFT trigger, Y for RIGHT trigger
			std::pair<Utilities::Vector2D, Utilities::Vector2D> joysticks;
		};

		bool initControllers();

		void addController(int which);

		void removeController(int which);

		void controllerButtonPressed(const SDL_Event& event);

		void controllerButtonReleased(const SDL_Event& event);

		void updateControllersButtons();

		void onControllerAxisMotion(const SDL_Event& event);

		void readSticksValues();


		std::unordered_map<int, ControllerData> controllers;

		bool isAxisMotionEvent_;
		bool isControllerButtonDownEvent_;
		bool isControllerButtonUpEvent_;
		bool controllerConnected_;
		bool controllerDisconnected_;

		int lastInputReceivedController;

		int numControllersConnected;
	};
}