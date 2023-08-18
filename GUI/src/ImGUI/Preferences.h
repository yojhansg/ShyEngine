#pragma once

#include "Window.h"
#include <string>

#include <nlohmann/json_fwd.hpp>

namespace PEditor {


	struct ProjectData {

		std::string name;
		std::string creator;

		std::string icon;
		std::string initialScene;


		std::string windowTitle;

		//Window
		int width;
		int height;

		bool splashScreen;
		bool vsync;

		//Physics
		bool fullScreen;
		bool closeWithEscape;

		float gravity_x;
		float gravity_y;

		float timeToDoubleClick;
		float timeToHoldClick;

		//Internal
		std::string path;

		bool debugPhysics;
		bool debugFramerate;
	};

	class Preferences : public Window {


	private:

		static Preferences* instance;
		bool open;
		bool visible;

		ProjectData data;

		nlohmann::ordered_json BasicData();

	public:

		Preferences();

		static void Open();

		static void LoadData();

		static void GenerateDebug();
		static void GenerateRelease();


		virtual void render();

		static ProjectData& GetData();
	};

}