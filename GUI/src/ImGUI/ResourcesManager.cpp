#include "ResourcesManager.h"

#include "Texture.h"
#include "Font.h"

#include "CheckML.h"


namespace ShyEditor {

	ResourcesManager* ResourcesManager::instance = nullptr;

	const std::string ResourcesManager::ASSETSFOLDER = "Assets\\";

	ResourcesManager::ResourcesManager() {
	
		editorResourcesPath = "";
		engineResourcesPath = ""; 

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

			texture = new Texture(ASSETSFOLDER + "Images\\" + key);

			instance->editorTextures.insert(std::make_pair(key, texture));

		}
		else {

			if (instance->engineTextures.contains(key))
				return instance->engineTextures.at(key);

			texture = new Texture(instance->engineResourcesPath + key);

			instance->engineTextures.insert(std::make_pair(key, texture));

		}

		return texture;

	}

	void ResourcesManager::Init()
	{
		instance = new ResourcesManager();
		instance->currentAsset.valid = false;
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

	Font* ResourcesManager::AddFont(const std::string& key, int pointSize) {

		if (instance->engineFonts.contains(key))
			return instance->engineFonts.at(key);

		Font* font = new Font(instance->engineResourcesPath + key, pointSize);

		instance->engineFonts.insert(std::make_pair(key, font));

		return font;

	}

	std::string ResourcesManager::GetEditorResourcesPath() {
		return instance->editorResourcesPath;
	}

	void ResourcesManager::SetEditorResourcesPath(const std::string& path) {
		instance->editorResourcesPath = path;
	}

	std::string ResourcesManager::GetEngineResourcesPath() {
		return instance->engineResourcesPath;
	}

	void ResourcesManager::SetEngineResourcesPath(const std::string& path) {
		instance->engineResourcesPath = path;
	}

	void ResourcesManager::UnselectAsset()
	{
		instance->currentAsset.valid = false;
	}

	void ResourcesManager::SelectAsset(const Asset& asset)
	{
		instance->currentAsset = asset;
	}

	bool ResourcesManager::IsAnyAssetSelected()
	{
		return instance->currentAsset.valid;
	}

	const Asset& ResourcesManager::SelectedAsset()
	{
		return instance->currentAsset;
	}

}