#pragma once

#include <string>
#include <vector>
#include <Vector2D.h>


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

	// Physics
	bool debugPhysics;
	bool debugFramerate;
	Utilities::Vector2D gravity;
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
	std::string resourcesPath;

};