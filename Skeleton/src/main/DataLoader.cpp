#include "DataLoader.h"
#include <json.hpp>
#include <fstream>

#include "ConsoleManager.h"

const std::string DataLoader::extension = ".json";

using namespace nlohmann;

DataLoader::DataLoader() {
	valid = false;
}

DataLoader::DataLoader(cstring game, cstring creator, cstring windowName, cstring windowIcon, Utilities::Vector2D const& windowSize,
	bool useSplashScreen, cstring initialScene, bool vsync, bool fullscreen, Utilities::Vector2D const& gravity, bool debugPhysics):
	game(game), creator(creator), windowName(windowName), windowIcon(windowIcon), windowSize(windowSize), useSplashScreen(useSplashScreen), initialScene(initialScene), vsyncEnabled(vsync),
	fullScreen(fullscreen), gravity(gravity), debugPhysics(debugPhysics)
{
	valid = true;
}


DataLoader DataLoader::Load(std::string const& path) {

	std::ifstream fileStream(path + extension);

	if (!fileStream.good())
	{
		return DataLoader();
	}

	json file = json::parse(fileStream);

	std::string game = file["game"].get<std::string>();
	std::string creator = file["creator"].get<std::string>();
	std::string windowTitle = file["windowTitle"].get<std::string>();
	std::string windowIcon = file["windowIcon"].get<std::string>();
	std::string initialScene = file["initialScene"].get<std::string>();

	Utilities::Vector2D windowSize = file["windowSize"].get<std::string>();
	Utilities::Vector2D gravity = file["gravity"].get<std::string>();

	bool splashScreen = file["splashScreen"].get<bool>();
	bool vsync = file["vsync"].get<bool>();
	bool fullScreen = file["fullScreen"].get<bool>();
	bool debugPhysics = file["debugPhysics"].get<bool>();

	fileStream.close();

	return DataLoader(game, creator, windowTitle, windowIcon, windowSize, splashScreen, initialScene, vsync, fullScreen, gravity, debugPhysics);
}