#pragma once

#include "SDL.h"
#include <Singleton.h>
#include <Vector2D.h>
#include <array>
#include <vector>

#define MAX_STICK_VALUE 32767
#define STICK_DEADZONE 3276
#define TRIGGER_DEADZONE 1000

namespace InputManager {

	class InputManager : public Singleton<InputManager> { 

		friend Singleton<InputManager>;

	public:

		enum MOUSEBUTTON : uint8_t {
			LEFT = 0, MIDDLE = 1, RIGHT = 2
		};

		enum CONTROLLERSTICK : uint8_t {
			LEFTSTICK = 0, RIGHTSTICK = 1,
		};

		enum CONTROLLERTRIGGER : uint8_t {
			LEFT_TRIGGER = 0, RIGHT_TRIGGER = 1,
		};

		enum PS4CONTROLLER_BUTTONS : uint8_t {
			X = 0, CIRCLE = 1, SQUARE = 2, TRIANGLE = 3, SHARE = 4, HOME = 5,  OPTIONS = 6, L3 = 7, 
			R3 = 8, L1 = 9, R1 = 10, UP_ARROW = 11, DOWN_ARROW = 12, LEFT_ARROW = 13
		};

		~InputManager();

		// clear the state
		void clearState();

		// update the state with a new event
		void update(const SDL_Event& event);

		// keyboard
		bool keyDownEvent();

		bool keyUpEvent();

		bool isKeyDown(SDL_Scancode key);

		bool isKeyDown(SDL_Keycode key);

		bool isKeyUp(SDL_Scancode key);

		bool isKeyUp(SDL_Keycode key);

		// mouse
		bool mouseMotionEvent();

		bool mouseButtonEvent();

		const std::pair<Sint32, Sint32>& getMousePos();

		int getMouseButtonState(MOUSEBUTTON b);

		// controller
		bool isJoystickAxisMotion();

		bool isJoystickButtonEventDown();

		bool isJoystickButtonEventUp();

		bool getJoystickId();

		Vector2D getJoystickValue(int joy, CONTROLLERSTICK ct);

		float getJoystickTriggerValue(int joy, CONTROLLERTRIGGER ct);
		
		bool getJoystickButtonState(int joy, int button);

	private:

		InputManager();

		// --------- MOUSE & KB ------------

		void onKeyDown(const SDL_Event& event);

		void onKeyUp(const SDL_Event& event);

		void onMouseMotion(const SDL_Event& event);

		void onMouseButtonChange(const SDL_Event& event, bool isDown);

		bool isKeyUpEvent_;
		bool isKeyDownEvent_;
		bool isMouseMotionEvent_;
		bool isMouseButtonEvent_;
		std::pair<Sint32, Sint32> mousePos_;
		std::array<bool, 3> mbState_;
		const Uint8* kbState_;


		// ----- CONTROLLER -------

		void initialiseJoysticks();

		void clearJoysticksButtons();

		void removeJoysticks();

		void onJoystickAxisMotion(const SDL_Event& event);

		void onJoystickButtonDown(const SDL_Event& event);

		void onJoystickButtonUp(const SDL_Event& event);

		std::vector<SDL_Joystick*> joysticks;
		std::vector<std::pair<Vector2D*, Vector2D*>> joystickValues;
		std::vector<std::pair<int, int>> joystickTriggerValues;
		std::vector<std::vector<bool>> joystickButtonStates;

		bool isAxisMotionEvent_;
		bool isJoystickButtonDownEvent_;
		bool isJoystickButtonUpEvent_;
		int joystickId;

	};

	// A macro to simplify the way to access the instance

	inline InputManager& im() {
		return *InputManager::instance();
	}
}