#include "ResourcesManager.h"

namespace ShyEditor {

	ResourcesManager* ResourcesManager::instance = nullptr;

	ResourcesManager::ResourcesManager() {

	}

	void ResourcesManager::AddEditorResource() {

	}

	void ResourcesManager::AddEngineResource() {

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

	const Asset& ResourcesManager::SelectedAsset()
	{
		return instance->currentAsset;
	}

}