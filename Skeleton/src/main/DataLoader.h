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
	bool fullScreen; //TODO: no se usa
	bool vsync; //TODO: no se usa
	bool useSplashScreen; //TODO: no se usa
	bool debugPhysics;
	bool debugFrameRate;
	bool closeWithEscape;

	float timeToDoubleClick;
	float timeToHoldClick;

	std::string game; //TODO: no se usa
	std::string creator; //TODO: no se usa
	std::string windowTitle;
	std::string windowIcon; //TODO: no se usa
	std::string initialScene; //TODO: no se usa

	Utilities::Vector2D windowSize;
	Utilities::Vector2D gravity;
};