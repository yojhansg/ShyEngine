#pragma once

#include <string>
#include <Vector2D.h>


class DataLoader {


private: 

	using cstring = std::string const&;

	static const std::string extension;

	DataLoader();
	DataLoader(cstring game, cstring creator, cstring windowName, cstring windowIcon, Utilities::Vector2D const& windowSize,
		bool useSplashScreen, cstring initialScene, bool vsync, bool fullscreen, Utilities::Vector2D const& gravity, bool debugPhysics);
	
public:
	static DataLoader Load(std::string const& path);

	bool valid;
	bool fullScreen; //TODO: no se usa
	bool vsyncEnabled; //TODO: no se usa
	bool useSplashScreen; //TODO: no se usa
	bool debugPhysics;

	std::string game; //TODO: no se usa
	std::string creator; //TODO: no se usa
	std::string windowName;
	std::string windowIcon; //TODO: no se usa
	std::string initialScene; //TODO: no se usa

	Utilities::Vector2D windowSize;
	Utilities::Vector2D gravity;
};