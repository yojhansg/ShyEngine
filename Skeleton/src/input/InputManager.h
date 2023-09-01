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

	EditorManager InputManager : public Utilities::Singleton<InputManager>{

		friend Singleton<InputManager>;

	public:

		enum class MOUSEBUTTON : uint8_t {
			LEFT, MIDDLE, RIGHT, Count
		};

		enum class CONTROLLERSTICK : uint8_t {
			LEFT_STICK, RIGHT_STICK
		};

		enum class CONTROLLERTRIGGER : uint8_t {
			LEFT_TRIGGER, RIGHT_TRIGGER, N_TRIGGERS
		};

		// PS4 Mapping
		enum class PS4_CONTROLLER_BUTTONS : uint8_t {
			X , CIRCLE , SQUARE , TRIANGLE, SHARE, HOME, OPTIONS, L3, R3,
			L1, R1, UP_ARROW, DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, PANEL, Count = 16
		};

		// XBOX Mapping
		enum class XBOX_CONTROLLER_BUTTONS : uint8_t {
			A, B, X, Y , BACK, GUIDE, START, LSB, RSB,
			LR, RB, UP_ARROW, DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, PANEL, Count = 16
		};

		enum class ButtonState {
			None, JustDown, Down, Up
		};

		// Keyboard

		enum class KB_LETTERS {
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, Count
		};

		enum class KB_NUMBERS {
			Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, Count
		};

		enum class KB_SPECIALKEYS {
			RETURN, ESCAPE, BACKSPACE, TAB, SPACE, RIGHT, LEFT, DOWN, UP, LCTRL, LSHIFT, LALT, RCTRL, RSHIFT, RALT, Count
		};

		~InputManager();

		bool Valid() override;

		// Called in the main loop to check is window is closed
		bool handleInput(SDL_Event& e);

		bool isKeyDown(SDL_Scancode key);

		bool isKeyUp(SDL_Scancode key);

		void UpdateKeyState(ButtonState& key);

		/*
			Nombres de eventos:

			Down: Una tecla esta siendo pulsada
			Up: Una tecla no esta siendo pulsada
			Pressed: Una tecla acaba de ser pulsada
			Released: Una tecla acaba de ser soltada
		*/

	publish:

		// Keyboard
		bool AnyKeyPressed();
		bool AnyKeyReleased();

		bool IsLetterPressed(int l);
		bool IsLetterDown(int l);
		bool IsLetterUp(int l);
		bool IsLetterReleased(int l);

		bool IsNumberPressed(int n);
		bool IsNumberDown(int n);
		bool IsNumberUp(int n);
		bool IsNumberReleased(int n);

		bool IsSpecialKeyPressed(int s);
		bool IsSpecialKeyDown(int s);
		bool IsSpecialKeyUp(int s);
		bool IsSpecialKeyReleased(int s);

		float HorizontalMovement();
		float VerticalMovement();


		// Mouse
		bool HasMouseMoved();

		bool HasMouseWheelMoved();

		bool IsMouseButtonDown(int b);

		bool IsMouseButtonUp(int b);

		bool IsMouseButtonPressed(int b);

		bool IsMouseButtonReleased(int b);

		cVector2D GetMousePosition();

		int GetMouseWheelScroll();


		// Controller
		int ConnectedControllersCount();

		bool AnyControllerButtonPressed();

		bool AnyControllerButtonReleased();

		bool AnyControllerAxisMotion();

		bool AnyControllerConnected();

		bool AnyControllerDisconnected();

		// With id

			bool IsControllerButtonPressedWithId(int button, int id);

			bool IsControllerButtonDownWithId(int button, int id);

			bool IsControllerButtonReleasedWithId(int button, int id);

			float GetLeftTriggerValueWithId(int id);

			float GetRightTriggerValueWithId(int id);

			bool IsLeftTriggerDownWithId(int id);

			bool IsRightTriggerDownWithId(int id);

			float GetLeftStickHorizontalValueWithId(int id);

			float GetLeftStickVerticalValueWithId(int id);

			float GetRightStickHorizontalValueWithId(int id);

			float GetRightStickVerticalValueWithId(int id);

			bool HasLeftStickMovedWithId(int id);

			bool HasRightStickMovedWithId(int id);


			// Without id

				bool IsControllerButtonPressed(int button);

				bool IsControllerButtonDown(int button);

				bool IsControllerButtonUp(int button);

				float GetLeftTriggerValue();

				float GetRightTriggerValue();

				bool IsLeftTriggerDown();

				bool IsRightTriggerDown();

				float GetLeftStickHorizontalValue();

				float GetLeftStickVerticalValue();

				float GetRightStickHorizontalValue();

				float GetRightStickVerticalValue();

				bool HasLeftStickMoved();

				bool HasRightStickMoved();



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