#pragma once

#include "SDL.h"
#include <Singleton.h>
#include <array>
#include <vector>
#include <string>
#include <Vector2D.h>
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
			LEFT_TRIGGER = 0, RIGHT_TRIGGER = 1
		};

		// PS4 Mapping
		enum class PS4_CONTROLLER_BUTTONS : uint8_t {
			X = 0, CIRCLE = 1, SQUARE = 2, TRIANGLE = 3, SHARE = 4, HOME = 5,  OPTIONS = 6, L3 = 7, R3 = 8, 
			L1 = 9, R1 = 10, UP_ARROW = 11, DOWN_ARROW = 12, LEFT_ARROW = 13, RIGHT_ARROW = 14, PANEL = 15
		};

		//XBOX Mapping
		enum class XBOX_CONTROLLER_BUTTONS : uint8_t {
			A = 0, B = 1, X = 2, Y = 3, SHARE = 4, HOME = 5, OPTIONS = 6, L3 = 7, R3 = 8,
			L1 = 9, R1 = 10, UP_ARROW = 11, DOWN_ARROW = 12, LEFT_ARROW = 13, RIGHT_ARROW = 14, PANEL = 15
		};

		enum class KeyState {

			None, JustDown, Down, Up
		};

		//Keyboard

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

		void UpdateKeyState(KeyState& key);

	publish:

		//Down Hold Up

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

		Utilities::Vector2D getMousePos();

		int getWheelMotionY();


		// Controller
		int getJoysticksConnected();

		bool isJoystickAxisMotion();

		bool isJoystickButtonEventDown();

		bool isJoystickButtonEventUp();

		// With id

			Utilities::Vector2D getJoystickValueWithId(int ct, int id);

			float getJoystickTriggerValueWithId(int ct, int id);
		
			bool getJoystickButtonStateWithId(int button, int id);

			int getJoysticksNumButtonsWithId(int id);

			bool isLeftJoystickMotionWithId(int id);

			bool isRightJoystickMotionWithId(int id);

			bool isLeftTriggerMotionWithId(int id);

			bool isRightTriggerMotionWithId(int id);

			float ControllerHorizontalMovementWithId(int id);

			float ControllerVerticalMovementWithId(int id);


		// Without id

			Utilities::Vector2D getJoystickValue(int ct);

			float getJoystickTriggerValue(int ct);

			bool getJoystickButtonState(int button);

			int getJoysticksNumButtons();

			bool isLeftJoystickMotion();

			bool isRightJoystickMotion();

			bool isLeftTriggerMotion();

			bool isRightTriggerMotion();

			float ControllerHorizontalMovement();

			float ControllerVerticalMovement();



		bool joystickConnectedEvent();

		bool joystickDisconnectedEvent();



	public:

		std::string getJoystickName(int id);

		bool getJoystickId();

	private:

		InputManager();
		InputManager(bool closeWithEscape);

		SDL_Scancode ConvertToScancode(const KB_LETTERS& letter);
		SDL_Scancode ConvertToScancode(const KB_NUMBERS& number);
		SDL_Scancode ConvertToScancode(const KB_SPECIALKEYS& specialKey);


		KB_LETTERS ConvertToLetter(const SDL_Scancode& scancode);
		KB_NUMBERS ConvertToNumbers(const SDL_Scancode& scancode);
		KB_SPECIALKEYS ConvertToSpecialKeys(const SDL_Scancode& scancode);


		bool valid;

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


		//Keyboard keys
		KeyState letters[(int)KB_LETTERS::Count];
		KeyState numbers[(int)KB_NUMBERS::Count];
		KeyState specialKeys[(int)KB_SPECIALKEYS::Count];


		// ----- CONTROLLER -------

		bool initialiseJoysticks();

		bool addJoystick(int joystick_id);

		void removeJoystick(int joystick_id);

		void clearJoysticksButtons();

		void removeJoysticks();

		void joystickConnected();

		void joystickDisconnected();

		void onJoystickAxisMotion(const SDL_Event& event);

		void onJoystickButtonDown(const SDL_Event& event);

		void onJoystickButtonUp(const SDL_Event& event);

		std::vector<SDL_Joystick*> joysticks;
		std::vector<std::pair<Utilities::Vector2D*, Utilities::Vector2D*>> joystickValues;
		std::vector<std::pair<int, int>> joystickTriggerValues;
		std::vector<std::vector<bool>> joystickButtonStates;
		std::vector<int> joystickNumButtons;
		std::vector<std::string> joystickNames;

		bool isAxisMotionEvent_;
		bool isJoystickButtonDownEvent_;
		bool isJoystickButtonUpEvent_;
		bool joystickConnected_;
		bool joystickDisconnected_;
		int joystickId;
		int numJoysticksConnected;
	};
}