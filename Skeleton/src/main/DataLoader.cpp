#include "DataLoader.h"
#include <json.hpp>
#include <fstream>

#include "ConsoleManager.h"

const std::string DataLoader::extension = ".json";

using namespace nlohmann;

DataLoader::DataLoader() {
	valid = false;
}


#define LoadValue(name, type, notFound) file.contains(name) ? file[name].get<type>() : notFound

DataLoader DataLoader::Load(std::string const& path) {

	std::ifstream fileStream(path + extension);

	if (!fileStream.good())
		return DataLoader();

	if (!json::accept(fileStream))
		return DataLoader();

	fileStream.clear();
	fileStream.seekg(0);

	json file = json::parse(fileStream);
	fileStream.close();

	DataLoader data;

	// General
	data.game = LoadValue("game", std::string, "MyGame");
	data.creator = LoadValue("creator", std::string, "Yojhan, Pablo e Ivan");
	data.initialScene = LoadValue("initialScene", std::string, "Assets\\DefaultScene");
	data.useSplashScreen = LoadValue("splashScreen", bool, true);

	// Window
	data.windowTitle = LoadValue("windowTitle", std::string, "Game");
	data.windowIcon = LoadValue("windowIcon", std::string, "Assets\\icon.png");
	data.windowSize = LoadValue("windowSize", std::string, "800, 800");
	data.vsync = LoadValue("vsync", bool, true);
	data.fullscreen = LoadValue("fullScreen", bool, false);
	data.showCursor = LoadValue("showcursor", bool, true);

	// Physics
	data.gravity = LoadValue("gravity", std::string, "0, 9.81");
	data.debugPhysics = LoadValue("debugPhysics", bool, false);
	data.debugFramerate = LoadValue("debugFrameRate", bool, false);
	data.collisionMatrix = LoadValue("matrix", std::vector<std::vector<bool>>, std::vector<std::vector<bool>>());
	data.layers = LoadValue("layers", std::vector<std::string>, std::vector<std::string>());

	// Input
	data.closeWithEscape = LoadValue("closeWithEscape", bool, true);

	// Overlay
	data.timeToHoldClick = LoadValue("timeToHoldClick", float, 0.1f);
	data.timeToDoubleClick = LoadValue("timeToDoubleClick", float, 0.5f);

	// Audio
	data.frequency = LoadValue("frequency", int, 44100);
	data.channels = LoadValue("channels", int, 2);
	data.chunksize = LoadValue("chunksize", int, 2048);
	
	// Internal
	data.resourcesPath = LoadValue("path", std::string, "");

	data.valid = true;

	return data;
}