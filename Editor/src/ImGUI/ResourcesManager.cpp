#include "ResourcesManager.h"

#include "Texture.h"
#include "Font.h"

#include "CheckML.h"


namespace ShyEditor {

	ResourcesManager* ResourcesManager::instance = nullptr;

	const std::string ResourcesManager::EDITORASSETSFOLDER = "Assets";
	const std::string ResourcesManager::EDITORENGINEFOLDER = "Engine";

	ResourcesManager::ResourcesManager() {
	
		engineProjectPath = "";

	}

	ResourcesManager* ResourcesManager::GetInstance() {

		if (instance == nullptr)
			instance = new ResourcesManager();

		return instance;
	}

	Texture* ResourcesManager::AddTexture(const std::string& key, bool isEditorResource) {

		Texture* texture = nullptr;

		if (isEditorResource) {

			if (instance->editorTextures.contains(key))
				return instance->editorTextures.at(key);

			texture = new Texture(EDITORASSETSFOLDER + "\\Images\\" + key);

			instance->editorTextures.insert(std::make_pair(key, texture));

		}
		else {

			if (instance->engineTextures.contains(key))
				return instance->engineTextures.at(key);

			texture = new Texture(instance->engineProjectPath + "\\Assets\\" + key);

			instance->engineTextures.insert(std::make_pair(key, texture));

		}

		return texture;

	}

	Font* ResourcesManager::AddFont(const std::string& key, int pointSize) {

		std::string newkey = key + std::to_string(pointSize);

		if (instance->engineFonts.contains(newkey))
			return instance->engineFonts.at(newkey);

		Font* font = new Font(instance->engineProjectPath + "\\Assets\\" + key, pointSize);

		instance->engineFonts.insert(std::make_pair(key, font));

		return font;

	}

	void ResourcesManager::Init()
	{
		instance = new ResourcesManager();
	}

	void ResourcesManager::Release() {

		// Editor textures release
		for (auto it = instance->editorTextures.begin(); it != instance->editorTextures.end(); it++) 
			delete it->second;

		instance->editorTextures.clear();

		// Engine textures release
		for (auto it = instance->engineTextures.begin(); it != instance->engineTextures.end(); it++)
			delete it->second;

		instance->engineTextures.clear();

		// Engine fonts release
		for (auto it = instance->engineFonts.begin(); it != instance->engineFonts.end(); it++)
			delete it->second;

		instance->engineFonts.clear();

		delete instance;
		instance = nullptr;
	}


	std::string ResourcesManager::GetProjectPath() {
		return instance->engineProjectPath;
	}

	void ResourcesManager::SetProjectPath(const std::string& path) {
		instance->engineProjectPath = path;
	}

}