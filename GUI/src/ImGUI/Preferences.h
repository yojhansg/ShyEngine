#pragma once

#include "Window.h"
#include <string>


namespace PEditor {


	class Preferences : public Window {


	private:
		static bool open;
		bool visible;

	public:

		Preferences();

		static void Open();

		virtual void render();

	};

}