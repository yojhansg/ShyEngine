#pragma once

#include <unordered_map>
#include <string>

namespace ShyEditor {

	template<typename T>
	using resources_map = std::unordered_map<std::string, T>;

	class Texture;
	class Font;

	class ResourcesManager {

	public:

		static const std::string EDITORASSETSFOLDER;
		static const std::string EDITORENGINEFOLDER;

		static ResourcesManager* GetInstance();

		Texture* AddTexture(const std::string& key, bool isEditorResource);
		Font* AddFont(const std::string& key, int pointSize);

		static std::string GetEngineResourcesPath();
		static void SetEngineResourcesPath(const std::string& path);

		static std::string GetProjectPath();
		static void SetProjectPath(const std::string& path);

		static void Init();
		static void Release();

	private:

		ResourcesManager();

		static ResourcesManager* instance;

		std::string engineProjectPath;

		resources_map<Texture*> editorTextures;

		resources_map<Texture*> engineTextures;
		resources_map<Font*> engineFonts;
	};

}
