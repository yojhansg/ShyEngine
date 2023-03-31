#include "ResourcesManager.h"
#include <RendererManager.h>
#include <Texture.h>
#include <Font.h>

Resources::ResourcesManager::ResourcesManager() {}

Resources::ResourcesManager::~ResourcesManager() {

	for (auto it = textures.begin(); it != textures.end(); it++) delete it->second;
	textures.clear();

	for (auto it = fonts.begin(); it != fonts.end(); it++) delete it->second;
	fonts.clear();
}

Renderer::Texture* Resources::ResourcesManager::addTexture(const std::string& key) {

	if (textures.contains(key))
		return textures.at(key);

	Renderer::Texture* t = new Renderer::Texture(key);

	textures.insert(std::make_pair(key, t));

	return t;

}

Renderer::Font* Resources::ResourcesManager::addFont(std::string const& key, int pointSize)
{
	std::string newkey = key + std::to_string(pointSize);

	if (fonts.contains(newkey))
		return fonts.at(newkey);

	Renderer::Font* t = new Renderer::Font(key, pointSize);

	fonts.insert(std::make_pair(newkey, t));

	return t;
}
