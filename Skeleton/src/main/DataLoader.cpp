#include "DataLoader.h"
#include <json.hpp>
#include <fstream>

#include "ConsoleManager.h"

const std::string DataLoader::extension = ".json";

using namespace nlohmann;

DataLoader::DataLoader() {
	valid = false;
}


DataLoader DataLoader::Load(std::string const& path) {

	std::ifstream fileStream(path + extension);

	if (!fileStream.good())
	{
		return DataLoader();
	}

	if (!json::accept(fileStream)) {

		return DataLoader();
	}

	fileStream.clear();
	fileStream.seekg(0);

	json file = json::parse(fileStream);
	fileStream.close();

	DataLoader data;

	data.game = file["game"].get<std::string>();
	data.creator = file["creator"].get<std::string>();
	data.windowTitle = file["windowTitle"].get<std::string>();
	data.windowIcon = file["windowIcon"].get<std::string>();
	data.initialScene = file["initialScene"].get<std::string>();

	data.windowSize = file["windowSize"].get<std::string>();
	data.gravity = file["gravity"].get<std::string>();

	data.useSplashScreen = file["splashScreen"].get<bool>();
	data.vsync = file["vsync"].get<bool>();
	data.fullScreen = file["fullScreen"].get<bool>();
	data.debugPhysics = file["debugPhysics"].get<bool>();

	data.debugFrameRate = file["debugFrameRate"].get<bool>();

	data.timeToHoldClick = file["timeToHoldClick"].get<float>();
	data.timeToDoubleClick = file["timeToDoubleClick"].get<float>();

	data.closeWithEscape = file["closeWithEscape"].get<bool>();


	data.valid = true;
	return data;
}