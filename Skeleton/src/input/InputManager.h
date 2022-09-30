#pragma once

#include "SDL.h"
#include <array>

class InputManager {

	public:

		enum MOUSEBUTTON : uint8_t {
			LEFT = 0, MIDDLE = 1, RIGHT = 2
		};

		InputManager();

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

	private:

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
};
