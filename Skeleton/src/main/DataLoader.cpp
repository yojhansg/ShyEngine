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

	data.game = LoadValue("game", std::string, "MyGame");
	data.creator = LoadValue("creator", std::string, "Yojhan, Pablo e Ivan");
	data.windowTitle = LoadValue("windowTitle", std::string, "Game");
	data.windowIcon = LoadValue("windowIcon", std::string, "Assets\\icon.png");
	data.initialScene = LoadValue("initialScene", std::string, "Assets\\DefaultScene");
	data.windowSize = LoadValue("windowSize", std::string, "800, 800");
	data.gravity = LoadValue("gravity", std::string, "0, 9.81");
	data.useSplashScreen = LoadValue("splashScreen", bool, true);
	data.vsync = LoadValue("vsync", bool, true);
	data.fullScreen = LoadValue("fullScreen", bool, false); 
	data.debugPhysics = LoadValue("debugPhysics", bool, false); 
	data.debugFrameRate = LoadValue("debugFrameRate", bool, false);
	data.closeWithEscape = LoadValue("closeWithEscape", bool, true);
	data.timeToHoldClick = LoadValue("timeToHoldClick", float, 0.1f); 
	data.timeToDoubleClick = LoadValue("timeToDoubleClick", float, 0.5f);
	data.resourcesPath = LoadValue("path", std::string, "");

	data.valid = true;
	return data;
}