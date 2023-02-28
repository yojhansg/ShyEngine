#pragma once

#include <Singleton.h>
#include <unordered_map>

namespace RendererManager {
	class Texture;
}

namespace ResourcesManager {

	template<typename T>
	using resources_map = std::unordered_map<std::string, T>;

	class ResourcesManager : public Utilities::Singleton<ResourcesManager> {

		friend Singleton<ResourcesManager>;

	public:

		~ResourcesManager();

		RendererManager::Texture* addTexture(const std::string& key);

	private:

		ResourcesManager();
		
		resources_map<RendererManager::Texture*> textures;
	};
}

