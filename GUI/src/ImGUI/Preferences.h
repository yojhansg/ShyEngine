#pragma once

#include "Window.h"
#include <string>


namespace PEditor {


	struct ProjectData {

		std::string name;
		std::string icon;
		std::string path;

		int windowWidht;
		int windowHeight;
	};

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