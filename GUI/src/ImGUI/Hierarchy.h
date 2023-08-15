#pragma once
#include <vector>
#include "Window.h"

union SDL_Event;

#define HIERARCHY_WIN_WIDTH_RATIO 0.4f
#define HIERARCHY_WIN_HEIGHT_RATIO 0.7f

namespace PEditor {
	class GameObject;

	class Hierarchy: public Window
	{
	private:

	public:
		
		Hierarchy();

		virtual void render();
	private:

		void showRenamePopup(GameObject* gameObject);
		void showGameObjectMenu(GameObject* gameObject);

		bool shouldOpenRenamePopup;
	};
}
