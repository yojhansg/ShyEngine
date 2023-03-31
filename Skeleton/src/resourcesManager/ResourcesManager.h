#pragma once

#include <Singleton.h>
#include <unordered_map>
#include <string>

namespace RendererManager {
	class Texture;
	class Font;
}

namespace ResourcesManager {

	template<typename T>
	using resources_map = std::unordered_map<std::string, T>;

	class ResourcesManager : public Utilities::Singleton<ResourcesManager> {

		friend Singleton<ResourcesManager>;

	public:

		~ResourcesManager();

		RendererManager::Texture* addTexture(const std::string& key);

		RendererManager::Font* addFont(std::string const& key);

	private:

		ResourcesManager();
		
		resources_map<RendererManager::Texture*> textures;
		resources_map<RendererManager::Font*> fonts;
	};
}

