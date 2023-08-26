#pragma once

#include "Window.h"

#include <string>
#include <nlohmann/json_fwd.hpp>

namespace ShyEditor {

	struct ProjectData {

		// General
		std::string initialScene;
		std::string windowTitle;
		std::string creator;

		// Window
		std::string name;
		std::string icon;
		int width;
		int height;
		bool vsync;
		bool fullscreen;
		bool showCursor;

		// Physics
		bool debugPhysics;
		bool debugFramerate;
		float gravity_x;
		float gravity_y;
		std::vector<std::vector<bool>> collisionMatrix;
		std::vector<std::string> layers;

		// Input
		bool closeWithEscape;

		// Overlay
		float timeToDoubleClick;
		float timeToHoldClick;

		// Audio
		int frequency;
		int channels;
		int chunksize;

		// Internal
		std::string buildPath;

	};


	class Preferences : public Window {

	private:

		static Preferences* instance;

		bool open;

		ProjectData data;

		void GeneralHeader();
		void WindowHeader();
		void AudioHeader();
		void InputHeader();
		void OverlayHeader();
		void PhysicsHeader();


		std::string newLayer;
		std::string defaultLayer;
		int nLayers;
		bool updateMatrix;
		bool showError;
		std::wstring errorMsg;

		nlohmann::ordered_json BasicData();

		// Shows a pop-up window with an error message
		void ShowErrorPopup(const std::wstring& errorMessage);

	public:

		Preferences();

		static void Open();

		static void LoadData();

		static void GenerateDebug();
		static void GenerateRelease();

		void Behaviour() override;

		static ProjectData& GetData();

	};

}