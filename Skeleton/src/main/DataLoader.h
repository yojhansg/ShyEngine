#pragma once

#include <string>
#include <vector>
#include <Vector2D.h>
#include <Color.h>


class DataLoader {

private: 

	using cstring = std::string const&;

	static const std::string extension;

	DataLoader();
	
public:

	static DataLoader Load(std::string const& path);

	bool valid;

	// General
	std::string initialScene;
	std::string creator;
	std::string game;
	bool useSplashScreen;

	// Window
	std::string windowTitle;
	std::string windowIcon;
	Utilities::Vector2D windowSize;
	bool vsync;
	bool fullscreen;
	bool showCursor;
	std::string bgColor;
	int bgAlpha;

	// Physics
	bool debugPhysics;
	bool debugFramerate;
	Utilities::Vector2D gravity;
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
	std::string resourcesPath;
	std::string projectFilePath;

};