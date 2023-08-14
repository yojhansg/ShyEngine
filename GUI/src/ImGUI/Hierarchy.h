#pragma once
#include <vector>
#include "Window.h"

union SDL_Event;
namespace PEditor {
	class Hierarchy: public Window
	{
	private:

	public:
		
		Hierarchy();

		virtual void render();
	};
}
