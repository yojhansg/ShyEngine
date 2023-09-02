#pragma once

#include "Window.h"
#include "Vector2.h"

#include <string>
#include <nlohmann/json_fwd.hpp>

struct ImVec2;

namespace ShyEditor {

	class Texture;

	struct PreferencesColor {

		PreferencesColor() {
			r = g = b = 0; a = 1;
		}

		float r;
		float g;
		float b;
		float a;
	};

	struct ProjectData {

		// General
		std::string initialScene;
		std::string windowTitle;
		std::string creator;
		bool splashScreen;

		// Window
		std::string name;
		std::string icon;
		int width;
		int height;
		bool vsync;
		bool fullscreen;
		bool showCursor;
		PreferencesColor bgColor;

		// Physics
		bool debugPhysics;
		bool debugFramerate;
		float gravity_x;
		float gravity_y;
		std::vector<std::vector<bool>> collisionMatrix;
		std::vector<std::string> layers;

		// Input
		bool closeWithEscape;
			
			// Keyboard
			int KB_Jump;
			int KB_Action;
			std::vector<int> KB_movements;

			// Controller

				// XBOX
				int XBOX_Jump;
				int XBOX_Action;

				// PS4
				int PS4_Jump;
				int PS4_Action;


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

		// Physics
		std::string newLayer;
		std::string defaultLayer;
		std::wstring errorMsg;
		bool updateMatrix;
		bool showError;
		int nLayers;

		// Input
		enum Arrows { LEFT, RIGHT, UP, DOWN };
		std::vector<std::string> specialKeys;
		std::vector<std::string> letterKeys;
		std::vector<std::string> xboxButtons;
		std::vector<std::string> ps4Buttons;
		Texture* upArrow;
		Texture* downArrow;
		Texture* leftArrow;
		Texture* rightArrow;

			// Keyboard

				// Jump
				std::string KB_jumpPreview;
				int KB_jumpSelected;

				// Movement
				int KB_movementSelected;

				// Action
				std::string KB_actionPreview;
				int KB_actionSelected;

			// Controller

				// XBOX

					// Jump
					std::string XBOX_jumpPreview;
					int XBOX_jumpSelected;

					// Action
					std::string XBOX_actionPreview;
					int XBOX_actionSelected;

				// PS4

					// Jump
					std::string PS4_jumpPreview;
					int PS4_jumpSelected;

					// Action
					std::string PS4_actionPreview;
					int PS4_actionSelected;


		void GeneralHeader();
		void WindowHeader();
		void AudioHeader();
		void InputHeader();
		void OverlayHeader();
		void PhysicsHeader();

		nlohmann::ordered_json BasicData();

		// Shows a pop-up window with an error message
		void ShowErrorPopup(const std::wstring& errorMessage);

		void ComboForArrow(int arrowIndex);

	public:

		Preferences();

		static void Open();

		static void StoreData();
		static void LoadData();

		static void GenerateDebug();
		static void GenerateRelease();
		static void GenerateBuild();

		void Behaviour() override;

		static ProjectData& GetData();

	};

}