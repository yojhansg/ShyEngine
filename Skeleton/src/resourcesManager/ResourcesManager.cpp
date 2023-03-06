#include "ResourcesManager.h"
#include <RendererManager.h>
#include <Texture.h>

ResourcesManager::ResourcesManager::ResourcesManager() {}

ResourcesManager::ResourcesManager::~ResourcesManager() {

	for (auto it = textures.begin(); it != textures.end(); it++) delete it->second;
	textures.clear();
}

RendererManager::Texture* ResourcesManager::ResourcesManager::addTexture(const std::string& key) {

	if (textures.contains(key))
		return textures.at(key);

	RendererManager::Texture* t = new RendererManager::Texture(key);

	textures.insert(std::make_pair(key, t));

	return t;

}
