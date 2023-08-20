#pragma once

#include <unordered_map>
#include <string>

namespace ShyEditor {

	template<typename T>
	using resources_map = std::unordered_map<std::string, T>;

	class ResourcesManager {

	public:

		enum AssetType {EDITOR_ASSET, ENGINE_ASSET};

		static void AddEditorResource();
		static void AddEngineResource();



		static std::string GetEditorResourcesPath();
		static void SetEditorResourcesPath(const std::string& path);

		static std::string GetEngineResourcesPath();
		static void SetEngineResourcesPath(const std::string& path);

	private:

		ResourcesManager();

		static ResourcesManager* instance;

		std::string editorResourcesPath;
		std::string engineResourcesPath;

	};

}