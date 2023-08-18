#pragma once

#include <string>
#include <Vector2D.h>


class DataLoader {


private: 

	using cstring = std::string const&;

	static const std::string extension;

	DataLoader();
	
public:
	static DataLoader Load(std::string const& path);

	bool valid;
	bool fullScreen; 
	bool vsync; 
	bool useSplashScreen; 
	bool debugPhysics;
	bool debugFrameRate;
	bool closeWithEscape;

	float timeToDoubleClick;
	float timeToHoldClick;

	std::string game; 
	std::string creator;
	std::string windowTitle;
	std::string windowIcon; 
	std::string initialScene; 

	std::string resourcesPath;

	Utilities::Vector2D windowSize;
	Utilities::Vector2D gravity;
};