#include "ResourcesManager.h"
#include <RendererManager.h>
#include <Texture.h>
#include <Font.h>

ResourcesManager::ResourcesManager::ResourcesManager() {}

ResourcesManager::ResourcesManager::~ResourcesManager() {

	for (auto it = textures.begin(); it != textures.end(); it++) delete it->second;
	textures.clear();

	for (auto it = fonts.begin(); it != fonts.end(); it++) delete it->second;
	fonts.clear();
}

RendererManager::Texture* ResourcesManager::ResourcesManager::addTexture(const std::string& key) {

	if (textures.contains(key))
		return textures.at(key);

	RendererManager::Texture* t = new RendererManager::Texture(key);

	textures.insert(std::make_pair(key, t));

	return t;

}

RendererManager::Font* ResourcesManager::ResourcesManager::addFont(std::string const& key)
{
	if (fonts.contains(key))
		return fonts.at(key);

	RendererManager::Font* t = new RendererManager::Font(key);

	fonts.insert(std::make_pair(key, t));

	return t;
}
